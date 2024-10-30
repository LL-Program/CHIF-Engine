import codecs
import configparser
import copy
import datetime
import glob
import gzip
import importlib
import math
import os
import pickle
import pprint
import queue
import re
import shutil
import sys
import types
import time
import traceback
import uuid

from collections import OrderedDict
from ctypes import *
from distutils.dir_util import copy_tree
from importlib.machinery import SourceFileLoader
from threading import Thread

from PIL import Image, ImageDraw, ImageFont, ImageFilter
import numpy as np
from numpy import array, float32, uint8
from OpenGL.GL import *

from CHIFEngineMain.Common import *
from CHIFEngineMain.Render import MaterialInstance, Triangle, Quad, Cube, Plane, Mesh, Model, Font
from CHIFEngineMain.Render import CreateProceduralTexture, NoiseTexture3D, CloudTexture3D, VectorFieldTexture3D
from CHIFEngineMain.Render import EffectInfo, ParticleInfo
from CHIFEngineMain.Render import FontData
from CHIFEngineMain.Render import SplinePoint, SplineData
from CHIFEngineMain.Render.Ocean.Constants import GRID_VERTEX_COUNT
from CHIFEngineMain.OpenGLContext import CreateTexture, Material, Texture2D, Texture2DArray, Texture3D, TextureCube
from CHIFEngineMain.OpenGLContext import Shader, ShaderCompileOption, ShaderCompileMessage, default_compile_option
from CHIFEngineMain.OpenGLContext import parsing_macros, parsing_uniforms, parsing_material_components
from CHIFEngineMain.Utilities import Attributes, Singleton, Config, Logger, Profiler, Float3
from CHIFEngineMain.Utilities import GetClassName, is_gz_compressed_file, check_directory_and_mkdir, get_modify_time_of_file
from . import Collada, OBJ, loadDDS, generate_font_data, TextureGenerator


class LoadingThread(Thread):
    def __init__(self, resource_manager):
        Thread.__init__(self)
        self.resource_manager = resource_manager
        self.running = True
        self.loading_queue = queue.Queue()
        self.complete_queue = queue.Queue()

    def push_loading(self, resource_name):
        self.loading_queue.put(resource_name)

    def run(self):
        while self.running:
            time.sleep(0.001)

            if not self.loading_queue.empty():
                resource_name = self.loading_queue.get()
                resource = self.resource_manager.mesh_loader.get_resource(resource_name)
                data = self.resource_manager.mesh_loader.load_resource_data(resource)
                if data is not None:
                    self.complete_queue.put((resource_name, data))

    def set_data(self):
        while not self.complete_queue.empty():
            resource_name, data = self.complete_queue.get()
            resource = self.resource_manager.mesh_loader.get_resource(resource_name)
            resource.set_data(data)


# -----------------------#
# CLASS : MetaData
# -----------------------#
class MetaData:
    def __init__(self, resource_version, resource_filepath, is_engine_resource):
        self.is_engine_resource = is_engine_resource
        self.filepath = os.path.splitext(resource_filepath)[0] + ".meta"
        self.resource_version = resource_version
        self.resource_filepath = resource_filepath
        self.resource_modify_time = get_modify_time_of_file(resource_filepath)
        self.source_filepath = ""
        self.source_modify_time = ""
        self.version_updated = False
        self.changed = False

        self.load_meta_file()

    def is_resource_file_changed(self):
        return self.resource_modify_time != get_modify_time_of_file(self.resource_filepath)

    def is_source_file_changed(self):
        return self.source_modify_time != get_modify_time_of_file(self.source_filepath)

    def set_resource_version(self, resource_version, save=True):
        self.changed |= self.resource_version != resource_version
        self.resource_version = resource_version
        if self.changed and save:
            self.save_meta_file()

    def set_resource_meta_data(self, resource_filepath, save=True):
        # filepath, ext = os.path.splitext(resource_filepath)
        # dirpath, filename = os.path.split(filepath)
        # resource_filepath = os.path.join(dirpath, filename.replace(".", os.sep) + ext)

        resource_modify_time = get_modify_time_of_file(resource_filepath)
        self.changed |= self.resource_filepath != resource_filepath
        self.changed |= self.resource_modify_time != resource_modify_time
        self.resource_filepath = resource_filepath
        self.resource_modify_time = resource_modify_time

        if self.changed and save:
            self.save_meta_file()

    def set_source_meta_data(self, source_filepath, save=True):
        # filepath, ext = os.path.splitext(source_filepath)
        # dirpath, filename = os.path.split(filepath)
        # source_filepath = os.path.join(dirpath, filename.replace(".", os.sep) + ext)

        source_modify_time = get_modify_time_of_file(source_filepath)
        self.changed |= self.source_filepath != source_filepath
        self.changed |= self.source_modify_time != source_modify_time
        self.source_filepath = source_filepath
        self.source_modify_time = source_modify_time

        if self.changed and save:
            self.save_meta_file()

    def load_meta_file(self):
        if os.path.exists(self.filepath):
            with open(self.filepath, 'r') as f:
                load_data = eval(f.read())
                resource_version = load_data.get("resource_version", None)
                resource_filepath = load_data.get("resource_filepath", None)
                resource_modify_time = load_data.get("resource_modify_time", None)
                source_filepath = load_data.get("source_filepath", None)
                source_modify_time = load_data.get("source_modify_time", None)

                self.changed |= self.resource_version != resource_version
                self.changed |= self.resource_filepath != resource_filepath
                self.changed |= self.resource_modify_time != resource_modify_time
                self.changed |= self.source_filepath != source_filepath
                self.changed |= self.source_modify_time != source_modify_time

                if resource_version is not None:
                    self.resource_version = resource_version
                if source_filepath is not None:
                    self.source_filepath = source_filepath
                if source_modify_time is not None:
                    self.source_modify_time = source_modify_time
        else:
            # save meta file
            self.changed = True

        if self.changed:
            self.save_meta_file()

    def save_meta_file(self):
        if (self.changed or not os.path.exists(self.filepath)) and os.path.exists(self.resource_filepath):
            with open(self.filepath, 'w') as f:
                save_data = dict(
                    resource_version=self.resource_version,
                    resource_filepath=self.resource_filepath,
                    resource_modify_time=self.resource_modify_time,
                    source_filepath=self.source_filepath,
                    source_modify_time=self.source_modify_time,
                )
                pprint.pprint(save_data, f)
            self.changed = False

    def delete_meta_file(self):
        if os.path.exists(self.filepath):
            os.remove(self.filepath)


# -----------------------#
# CLASS : Resource
# -----------------------#
class Resource:
    def __init__(self, resource_name, resource_type_name):
        self.name = resource_name
        self.type_name = resource_type_name
        self.data = None
        self.meta_data = None

    def get_resource_info(self):
        return self.name, self.type_name, self.data is not None

    def is_need_to_load(self):
        return self.data is None or self.meta_data.is_resource_file_changed()

    def set_data(self, data):
        if self.data is None:
            self.data = data
        else:
            # copy of data
            if type(data) in (dict, types.ModuleType):
                self.data = data
            else:
                self.data.__dict__ = data.__dict__

        # Notify that data has been loaded.
        ResourceManager.instance().core_manager.send_resource_info(self.get_resource_info())

    def delete_data(self):
        if self.data is not None and hasattr(self.data, 'delete'):
            self.data.delete()
        self.data = None

    def clear_data(self):
        self.data = None

    def get_data(self, checkLoading=True):
        if checkLoading and self.is_need_to_load():
            ResourceManager.instance().load_resource(self.name, self.type_name)
        return self.data

    def get_attribute(self):
        if self.data is not None:
            if hasattr(self.data, 'get_attribute'):
                return self.data.get_attribute()
            elif type(self.data) is dict:
                new_attributes = Attributes()
                new_attributes.set_attribute('name', self.name)

                # dict to attribute
                # attribute_names = list(self.data.keys())
                # attribute_names.sort()
                # for attribute_name in attribute_names:
                #     new_attributes.set_attribute(attribute_name, self.data[attribute_name])
                return new_attributes
        return None

    def set_attribute(self, attribute_name, attribute_value, item_info_history, attribute_index):
        if self.data and hasattr(self.data, 'set_attribute'):
            self.data.set_attribute(attribute_name, attribute_value, item_info_history, attribute_index)

    def add_component(self, attribute_name, parent_info, attribute_index):
        if self.data and hasattr(self.data, 'add_component'):
            self.data.add_component(attribute_name, parent_info, attribute_index)

    def delete_component(self, attribute_name, parent_info, attribute_index):
        if self.data and hasattr(self.data, 'delete_component'):
            self.data.delete_component(attribute_name, parent_info, attribute_index)


# -----------------------#
# CLASS : ResourceLoader
# -----------------------#
class ResourceLoader(object):
    name = "ResourceLoader"
    resource_dir_name = ''  # example : Fonts, Shaders, Meshes
    resource_version = 0
    resource_type_name = 'None'
    fileExt = '.*'
    externalFileExt = {}  # example, { 'WaveFront': '.obj' }
    USE_FILE_COMPRESS_TO_SAVE = True
    enable_basic_mode = True

    def __init__(self, resource_manager):
        self.resource_manager = resource_manager
        self.core_manager = resource_manager.core_manager
        self.scene_manager = resource_manager.core_manager.scene_manager
        self.sound_manager = resource_manager.core_manager.sound_manager

        self.engine_resource_path = os.path.join(resource_manager.engine_path, self.resource_dir_name)
        check_directory_and_mkdir(self.engine_resource_path)

        self.project_resource_path = os.path.join(resource_manager.project_path, self.resource_dir_name)
        check_directory_and_mkdir(self.project_resource_path)

        self.external_paths = []
        self.external_paths.append(os.path.join(resource_manager.engine_path, 'Externals', self.resource_dir_name))
        self.external_paths.append(os.path.join(resource_manager.project_path, 'Externals', self.resource_dir_name))
        for external_path in self.external_paths:
            check_directory_and_mkdir(external_path)

        self.resources = {}
        self.metaDatas = {}

    def close(self):
        pass

    @staticmethod
    def get_resource_name(resource_path, filepath, make_lower=True):
        resource_name = os.path.splitext(os.path.relpath(filepath, resource_path))[0]
        resource_name = resource_name.replace(os.sep, ".")
        return resource_name if make_lower else resource_name

    def is_new_external_data(self, meta_data, source_filepath):
        if os.path.exists(source_filepath):
            # Refresh the resource from external file.
            source_modify_time = get_modify_time_of_file(source_filepath)
            return (meta_data.resource_version != self.resource_version) or (meta_data.source_filepath == source_filepath and meta_data.source_modify_time != source_modify_time)
        else:
            return False

    def is_engine_resource(self, filepath):
        return filepath.startswith(self.engine_resource_path) or self.engine_resource_path == self.project_resource_path

    def is_engine_external(self, filepath):
        return filepath.startswith(self.resource_manager.engine_path) or self.resource_manager.engine_path == self.resource_manager.project_path

    def initialize(self):
        logger.info("initialize " + GetClassName(self))

        resource_paths = [self.engine_resource_path, ]
        if self.project_resource_path not in resource_paths:
            resource_paths.append(self.project_resource_path)

        # collect resource files
        for resource_path in resource_paths:
            is_engine_resource = resource_path is self.engine_resource_path
            for dirname, dirnames, filenames in os.walk(resource_path):
                for filename in filenames:
                    fileExt = os.path.splitext(filename)[1]
                    if ".*" == self.fileExt or fileExt == self.fileExt:
                        filepath = os.path.join(dirname, filename)
                        resource_name = self.get_resource_name(resource_path, filepath)
                        self.create_resource(resource_name=resource_name, resource_data=None, resource_filepath=filepath, is_engine_resource=is_engine_resource)

        # Convert external files to resources.
        if self.externalFileExt:
            # gather external source files
            for external_path in self.external_paths:
                is_engine_external = self.is_engine_external(external_path)
                externalFileList = []

                for dirname, dirnames, filenames in os.walk(external_path):
                    for filename in filenames:
                        source_filepath = os.path.join(dirname, filename)

                        file_ext = os.path.splitext(source_filepath)[1].lower()
                        if file_ext in self.externalFileExt.values() and source_filepath not in externalFileList:
                            externalFileList.append(source_filepath)

                # convert external file to rsource file.
                for source_filepath in externalFileList:
                    resource_name = self.get_resource_name(external_path, source_filepath)
                    resource = self.get_resource(resource_name, noWarn=True)
                    meta_data = self.get_meta_data(resource_name, noWarn=True)
                    # Create the new resource from exterial file.
                    if resource is None:
                        logger.info("Create the new resource from %s." % source_filepath)
                        resource = self.create_resource(resource_name, is_engine_resource=is_engine_external)
                        self.convert_resource(resource, source_filepath)
                    elif meta_data and self.is_new_external_data(meta_data, source_filepath):
                        self.convert_resource(resource, source_filepath)
                        logger.info("Refresh the new resource from %s." % source_filepath)

        # clear gabage meta file
        for dirname, dirnames, filenames in os.walk(self.project_resource_path):
            for filename in filenames:
                file_ext = os.path.splitext(filename)[1]
                if file_ext == '.meta':
                    filepath = os.path.join(dirname, filename)
                    resource_name = self.get_resource_name(self.project_resource_path, filepath)
                    resource = self.get_resource(resource_name, noWarn=True)
                    meta_data = self.get_meta_data(resource_name, noWarn=True)
                    if resource is None:
                        if meta_data:
                            meta_data.delete_meta_file()
                            self.metaDatas.pop(resource_name)
                        else:
                            logger.info("Delete the %s." % filepath)
                            os.remove(filepath)

    def get_new_resource_name(self, prefix=""):
        if prefix not in self.resources:
            return prefix

        num = 0
        while True:
            new_name = "%s_%d" % (prefix or self.resource_type_name, num)
            if new_name not in self.resources:
                return new_name
            num += 1
        return ''

    def convert_resource(self, resource, source_filepath):
        logger.warn("convert_resource is not implemented in %s." % self.name)

    def hasResource(self, resource_name):
        return resource_name in self.resources

    def get_resource(self, resource_name, noWarn=False):
        if resource_name in self.resources:
            return self.resources[resource_name]
        if not noWarn and resource_name:
            logger.error("%s cannot found %s resource." % (self.name, resource_name))
        return None

    def get_resource_data(self, resource_name, noWarn=False, checkLoading=True):
        resource = self.get_resource(resource_name, noWarn)
        return resource.get_data(checkLoading) if resource else None

    def get_resource_list(self):
        return list(self.resources.values())

    def get_resource_name_list(self):
        return list(self.resources.keys())

    def get_resource_attribute(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            return resource.get_attribute()
        return None

    def set_resource_attribute(self, resource_name, attribute_name, attribute_value, item_info_history, attribute_index):
        # rename resource
        if attribute_name == 'name':
            self.rename_resource(resource_name, attribute_value)
        else:
            # set other attributes
            resource = self.get_resource(resource_name)
            if resource:
                resource.set_attribute(attribute_name, attribute_value, item_info_history, attribute_index)

    def add_resource_component(self, resource_name, attribute_name, parent_info, attribute_index):
        resource = self.get_resource(resource_name)
        if resource:
            resource.add_component(attribute_name, parent_info, attribute_index)

    def delete_resource_component(self, resource_name, attribute_name, parent_info, attribute_index):
        resource = self.get_resource(resource_name)
        if resource:
            resource.delete_component(attribute_name, parent_info, attribute_index)

    def get_meta_data(self, resource_name, noWarn=False):
        if resource_name in self.metaDatas:
            return self.metaDatas[resource_name]
        if not noWarn:
            logger.error("Not found meta data of %s." % resource_name)
        return None

    def create_resource(self, resource_name, resource_data=None, resource_filepath=None, is_engine_resource=False):
        if resource_name in self.resources:
            # logger.warn('Resource name is duplicated. %s' % resource_name)
            resource_name = self.get_new_resource_name(resource_name)
        resource = Resource(resource_name, self.resource_type_name)
        if resource_data is not None:
            resource.set_data(resource_data)
        if resource_filepath is not None:
            is_engine_resource = self.is_engine_resource(resource_filepath)
        else:
            resource_filepath = self.engine_resource_path if is_engine_resource else self.project_resource_path
            resource_filepath = os.path.join(resource_filepath, resource_name.replace(".", os.sep)) + self.fileExt
        meta_data = MetaData(self.resource_version, resource_filepath, is_engine_resource)
        self.regist_resource(resource, meta_data)
        return resource

    def regist_resource(self, resource, meta_data=None):
        logger.info("Regist %s : %s" % (self.resource_type_name, resource.name))
        self.resources[resource.name] = resource
        if meta_data is not None:
            self.metaDatas[resource.name] = meta_data
            resource.meta_data = meta_data
        # The new resource registered.
        if resource:
            self.core_manager.send_resource_info(resource.get_resource_info())

    def unregist_resource(self, resource):
        if resource:
            if resource.name in self.metaDatas:
                self.metaDatas.pop(resource.name)
            if resource.name in self.resources:
                self.resources.pop(resource.name)
            self.core_manager.notify_delete_resource(resource.get_resource_info())

    def rename_resource(self, resource_name, new_name):
        if new_name and resource_name != new_name:
            resource_data = self.get_resource_data(resource_name)
            meta_data = self.get_meta_data(resource_name)
            resource = self.create_resource(new_name, resource_data, is_engine_resource=meta_data.is_engine_resource)
            if resource:
                if resource_data and hasattr(resource_data, 'name'):
                    resource_data.name = resource.name
                self.save_resource(resource.name)
                if resource.name != resource_name:
                    self.delete_resource(resource_name)
                logger.info("rename_resource : %s to %s" % (resource_name, new_name))

    def load_resource(self, resource_name):
        logger.warn("load_resource is not implemented in %s." % self.name)

    def unload_resource(self, resource_name):
        logger.warn("unload_resource is not implemented in %s." % self.name)

    def action_resource(self, resource_name):
        logger.warn("action_resource is not implemented in %s." % self.name)

    def duplicate_resource(self, resource_name):
        logger.warn("duplicate_resource is not implemented in %s." % self.name)
        # meta_data = self.get_meta_data(resource_name)
        # new_resource = self.create_resource(resource_name)
        # new_meta_data = self.get_meta_data(new_resource.name)
        #
        # if os.path.exists(meta_data.source_filepath) and not os.path.exists(new_meta_data.source_filepath):
        #     shutil.copy(meta_data.source_filepath, new_meta_data.source_filepath)
        #     self.load_resource(new_resource.name)
        #     logger.info("duplicate_resource : %s to %s" % (resource_name, new_resource_name))

    def save_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        resource_data = self.get_resource_data(resource_name)
        if resource is not None and resource_data is not None:
            if hasattr(resource_data, 'get_save_data'):
                save_data = resource_data.get_save_data()
                self.save_resource_data(resource, save_data)
                return True
        logger.warn("save_resource is not implemented in %s." % self.name)
        return False

    @staticmethod
    def load_resource_data(resource):
        filePath = ''
        if resource is not None:
            filePath = resource.meta_data.resource_filepath
            try:
                if os.path.exists(filePath):
                    # Load data (deserialize)
                    if is_gz_compressed_file(filePath):
                        with gzip.open(filePath, 'rb') as f:
                            load_data = pickle.load(f)
                    else:
                        # human readable data
                        with open(filePath, 'r') as f:
                            load_data = eval(f.read())
                    return load_data
            except:
                logger.error(traceback.format_exc())
        logger.error("file open error : %s" % filePath)
        return None

    def save_resource_data(self, resource, save_data, source_filepath=""):
        save_filepath = resource.name.replace('.', os.sep)

        if resource.meta_data.is_engine_resource:
            save_filepath = os.path.join(self.engine_resource_path, save_filepath) + self.fileExt
        else:
            save_filepath = os.path.join(self.project_resource_path, save_filepath) + self.fileExt

        save_dir = os.path.dirname(save_filepath)
        if not os.path.exists(save_dir):
            os.makedirs(save_dir)

        if self.save_data_to_file(save_filepath, save_data):
            # refresh meta data because resource file saved.
            resource.meta_data.set_resource_meta_data(save_filepath, save=False)
            resource.meta_data.set_source_meta_data(source_filepath, save=False)
            resource.meta_data.set_resource_version(self.resource_version, save=False)
            resource.meta_data.save_meta_file()

    def save_data_to_file(self, save_filepath, save_data):
        logger.info("Save : %s" % save_filepath)
        try:
            # store data, serialize
            if self.USE_FILE_COMPRESS_TO_SAVE:
                with gzip.open(save_filepath, 'wb') as f:
                    pickle.dump(save_data, f, protocol=pickle.HIGHEST_PROTOCOL)
            else:
                # human readable data
                with open(save_filepath, 'w') as f:
                    pprint.pprint(save_data, f, width=128)
            return True
        except:
            logger.error(traceback.format_exc())
        return False

    def delete_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource is not None:
            if resource.name in self.metaDatas:
                resource_filepath = self.metaDatas[resource.name].resource_filepath
            else:
                resource_filepath = ""
            if os.path.exists(resource_filepath):
                os.remove(resource_filepath)
                logger.info("Deleted the %s." % resource_filepath)
            self.unregist_resource(resource)


# ---------------------------#
# CLASS : ShaderLoader
# ---------------------------#
class ShaderLoader(ResourceLoader):
    name = "ShaderLoader"
    resource_dir_name = 'Shaders'
    resource_type_name = 'Shader'
    resource_version = 0.6
    fileExt = '.glsl'
    shader_version = "#version 430 core"
    enable_basic_mode = False

    def get_shader_version(self):
        return self.shader_version

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            if resource.is_need_to_load():
                file_path = resource.meta_data.resource_filepath
                if os.path.exists(file_path):
                    try:
                        f = codecs.open(file_path, mode='r', encoding='utf-8')
                        shader_code = f.read()
                        f.close()

                        shader = Shader(resource.name, shader_code)
                        resource.set_data(shader)
                        resource.meta_data.set_resource_meta_data(resource.meta_data.resource_filepath)
                        self.resource_manager.material_loader.reload_materials(resource.meta_data.resource_filepath)
                        return True
                    except:
                        logger.error(traceback.format_exc())
                        logger.error("Failed %s file open" % file_path)
            else:
                # do not need to load
                return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def action_resource(self, resource_name):
        shader = self.get_resource_data(resource_name)
        if shader:
            self.resource_manager.material_instance_loader.create_material_instance(resource_name=resource_name,
                                                                                    shader_name=resource_name,
                                                                                    macros={})

    def save_data_to_file(self, save_filepath, save_data):
        logger.info("Save : %s" % save_filepath)
        try:
            # human readable data
            with open(save_filepath, 'w') as f:
                f.write(save_data)
            return True
        except BaseException:
            logger.error(traceback.format_exc())
        return False


# -----------------------#
# CLASS : MaterialLoader
# -----------------------#
class MaterialLoader(ResourceLoader):
    name = "MaterialLoader"
    resource_dir_name = 'Materials'
    resource_type_name = 'Material'
    fileExt = '.mat'
    resource_version = 0.6
    USE_FILE_COMPRESS_TO_SAVE = False
    enable_basic_mode = False

    def __init__(self, resource_manager):
        ResourceLoader.__init__(self, resource_manager)
        # self.linked_material_map = {}

    def action_resource(self, resource_name):
        material = self.get_resource_data(resource_name)
        if material:
            self.resource_manager.material_instance_loader.create_material_instance(resource_name=material.shader_name,
                                                                                    shader_name=material.shader_name,
                                                                                    macros=material.macros)

    def reload_materials(self, shader_filepath):
        reload_shader_names = []
        resource_names = list(self.resources.keys())
        for resource_name in resource_names:
            reload = False
            meta_data = self.resources[resource_name].meta_data
            if meta_data:
                if shader_filepath == meta_data.source_filepath:
                    reload = True
                elif meta_data and hasattr(meta_data, 'include_files'):
                    for include_file in meta_data.include_files:
                        if shader_filepath == include_file:
                            reload = True
                            break
            if reload:
                self.load_resource(resource_name)
                material = self.get_resource_data(resource_name)
                if material and material.shader_name not in reload_shader_names:
                    reload_shader_names.append(material.shader_name)

        for shader_name in reload_shader_names:
            self.resource_manager.material_instance_loader.reload_material_instances(shader_name)

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            material_datas = self.load_resource_data(resource)
            if material_datas:
                meta_data = resource.meta_data
                generate_new_material = False
                if self.is_new_external_data(meta_data, meta_data.source_filepath):
                    generate_new_material = True

                # set include files meta datas
                meta_data.include_files = material_datas.get('include_files', {})
                for include_file in meta_data.include_files:
                    if get_modify_time_of_file(include_file) != meta_data.include_files[include_file]:
                        generate_new_material = True
                        break

                if generate_new_material:
                    shader_name = material_datas.get('shader_name')
                    macros = material_datas.get('macros', {})
                    self.generate_new_material(resource.name, shader_name, default_compile_option, macros)
                else:
                    material = Material(resource.name, material_datas)
                    resource.set_data(material)
                return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def save_to_binary_own(self):
        if not self.program:
            raise RuntimeError("Shader program is not valid.")

        # Initialize size to hold the length of the binary data
        size = ctypes.c_int()

        # Get the binary length
        glGetProgramiv(self.program, GL_PROGRAM_BINARY_LENGTH, ctypes.byref(size))
        self.check_gl_error()  # Check for OpenGL errors

        # Create a buffer for the binary data
        binary_data = ctypes.create_string_buffer(size.value)

        # Retrieve the binary data and format
        binary_format = ctypes.c_int()  # Create a variable for the format
        glGetProgramiv(self.program, GL_PROGRAM_BINARY_FORMATS, ctypes.byref(binary_format))
        self.check_gl_error()  # Check for OpenGL errors

        # Get the actual binary data
        glGetProgramBinary(self.program, size, None, ctypes.byref(binary_format), binary_data)
        self.check_gl_error()  # Check for OpenGL errors

        return binary_format.value, binary_data.raw  # Return the format and raw binary data

    def check_gl_error(self):
        error = glGetError()
        if error != GL_NO_ERROR:
            raise RuntimeError(f"OpenGL error: {error}")
    @staticmethod
    def generate_material_name(shader_name, macros=None):
        if macros is not None and 0 < len(macros):
            keys = sorted(macros.keys())
            add_name = [key + "_" + str(macros[key]) for key in keys]
            shader_name += "_" + str(uuid.uuid3(uuid.NAMESPACE_DNS, "_".join(add_name))).replace("-", "_")
        return shader_name

    def generate_new_material(self, material_name, shader_name, compile_option, macros={}):
        logger.info("Generate new material : %s" % material_name)
        shader = self.resource_manager.get_shader(shader_name)
        shader_version = self.resource_manager.get_shader_version()
        shader_meta_data = self.resource_manager.shader_loader.get_meta_data(shader_name)

        if shader is not None and shader_meta_data is not None:
            is_engine_resource = self.resource_manager.shader_loader.is_engine_resource(shader_meta_data.resource_filepath)
            engine_shader_directory = self.resource_manager.shader_loader.engine_resource_path
            project_shader_directory = self.resource_manager.shader_loader.project_resource_path

            shader_codes = shader.generate_shader_codes(is_engine_resource, engine_shader_directory, project_shader_directory, shader_version, compile_option, macros)
            if shader_codes is not None:
                shader_code_list = shader_codes.values()
                final_macros = parsing_macros(shader_code_list)
                uniforms = parsing_uniforms(shader_code_list)
                material_components = parsing_material_components(shader_code_list)

                final_material_name = material_name

                # final_material_name = self.generate_material_name(shader_name, final_macros)
                # Check the material_name with final_material_name.
                # if material_name != final_material_name:
                #     logger.warn("Generated material name is changed. : %s" % final_material_name)
                #     self.linked_material_map[material_name] = final_material_name
                #     self.delete_resource(material_name)

                include_files = {}
                for include_file in shader.include_files:
                    include_files[include_file] = get_modify_time_of_file(include_file)

                material_datas = dict(
                    shader_name=shader_name,
                    shader_codes=shader_codes,
                    include_files=include_files,
                    uniforms=uniforms,
                    material_components=material_components,
                    binary_data=None,
                    binary_format=None,
                    macros=final_macros
                )

                # set default uniform datas
                root_material = self.get_resource_data(shader_name, checkLoading=False)
                if root_material is not None:
                    material_datas['uniform_datas'] = copy.deepcopy(root_material.get_save_data()['uniform_datas'])

                # create material
                material = Material(final_material_name, material_datas)

                if material:
                    if material.valid:
                        resource = self.get_resource(final_material_name, noWarn=True)
                        if resource is None:
                            resource = self.create_resource(final_material_name, is_engine_resource=is_engine_resource)

                        # set include files meta datas
                        resource.meta_data.include_files = material_datas.get('include_files', {})

                        # write material to file, and regist to resource manager
                        shader_meta_data = self.resource_manager.shader_loader.get_meta_data(shader_name)
                        if shader_meta_data:
                            source_filepath = shader_meta_data.resource_filepath
                        else:
                            source_filepath = ""

                        # save binary data of shader.
                        # binary_format, binary_data = material.save_to_binary()
                        # if binary_format is not None and binary_data is not None:
                        #     material_datas['binary_format'] = binary_format
                        #     material_datas['binary_data'] = binary_data

                        # Done : save material data
                        # self.save_resource_data(resource, material_datas, source_filepath)
                        resource.set_data(material)
                        return material
                    else:
                        if ShaderCompileMessage.TEXTURE_NO_MATCHING_OVERLOADED_FUNCTION in material.compile_message:
                            logger.error("Recompile %s material cause global_texture_function_error." % material_name)
                            compile_option = []  # pop USE_GLOBAL_TEXTURE_FUNCTION compile option.
                            self.generate_new_material(material_name, shader_name, compile_option, macros=macros)
        logger.error("Failed to generate_new_material %s." % material_name)
        return None

    def get_material(self, shader_name, macros={}):
        if shader_name == '':
            logger.error("Error : Cannot create material. Because material name is empty.")
            return None

        material_name = self.generate_material_name(shader_name, macros)

        # Due to options such as macros, actual material names may differ. That's why we use link maps.
        # if material_name in self.linked_material_map:
        #     material_name = self.linked_material_map[material_name]

        material = self.get_resource_data(material_name)
        if material is None:
            material = self.generate_new_material(material_name, shader_name, default_compile_option, macros=macros)
        return material


# -----------------------#
# CLASS : MaterialInstanceLoader
# -----------------------#
class MaterialInstanceLoader(ResourceLoader):
    name = "MaterialInstanceLoader"
    resource_dir_name = 'MaterialInstances'
    resource_type_name = 'MaterialInstance'
    fileExt = '.matinst'
    USE_FILE_COMPRESS_TO_SAVE = False
    enable_basic_mode = False

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            material_instance_data = self.load_resource_data(resource)
            if material_instance_data:
                shader_name = material_instance_data.get('shader_name', 'default')
                macros = material_instance_data.get('macros', {})
                material = self.resource_manager.get_material(shader_name, macros)
                material_instance_data['material'] = material

                material_instance = MaterialInstance(resource.name, **material_instance_data)
                if material_instance.valid:
                    resource.set_data(material_instance)
                    if material_instance.isNeedToSave:
                        self.save_resource(resource_name)
                        material_instance.isNeedToSave = False
                return material_instance.valid
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def action_resource(self, resource_name):
        self.core_manager.request(COMMAND.VIEW_MATERIAL_INSTANCE, resource_name)

    def reload_material_instances(self, shader_name):
        for resource_name in self.resources:
            resource = self.resources[resource_name]
            if resource and resource.data:
                material_instance = resource.data
                if material_instance.shader_name == shader_name:
                    self.load_resource(resource_name)

        for resource_name in self.resources:
            resource = self.resources[resource_name]

    def create_material_instance(self, resource_name, shader_name, macros={}):
        if shader_name == '':
            shader_name = resource_name

        if shader_name:
            resource_name = self.get_new_resource_name(resource_name)
            material = self.resource_manager.get_material(shader_name, macros)
            material_instance = MaterialInstance(resource_name, material=material, shader_name=shader_name,
                                                 macros=macros)
            if material_instance.valid:
                resource = self.create_resource(resource_name)
                resource.set_data(material_instance)
                self.save_resource(resource_name)
                return True
        logger.error('Failed to %s material instance.' % resource_name)
        return False

    def get_material_instance(self, name, shader_name='', macros={}):
        material_instance = self.get_resource_data(name)
        if material_instance is None:
            if self.create_material_instance(resource_name=name,
                                             shader_name=shader_name,
                                             macros=macros):
                material_instance = self.get_resource_data(name)
            else:
                material_instance = self.get_resource_data('default')
        else:
            for macro, value in macros.items():
                if macro not in material_instance.macros or value != material_instance.macros[macro]:
                    material = self.resource_manager.get_material(material_instance.shader_name, macros)
                    material_instance.set_material(material)
            return material_instance
        return material_instance


# -----------------------#
# CLASS : TextureLoader
# -----------------------#
class TextureLoader(ResourceLoader):
    name = "TextureLoader"
    resource_dir_name = 'Textures'
    resource_type_name = 'Texture'
    resource_version = 2
    USE_FILE_COMPRESS_TO_SAVE = True
    enable_basic_mode = False
    fileExt = '.texture'
    externalFileExt = dict(GIF=".gif", JPG=".jpg", JPEG=".jpeg", PNG=".png", BMP=".bmp", TGA=".tga", TIF=".tif",
                           TIFF=".tiff", DXT=".dds", KTX=".ktx", PGM=".pgm")

    def __init__(self, resource_manager):
        ResourceLoader.__init__(self, resource_manager)
        self.new_texture_list = []

    def initialize(self):
        ResourceLoader.initialize(self)
        if not self.core_manager.is_basic_mode:
            self.generate_cube_textures()

            # generate common textures
            TextureGenerator.generate_common_textures(self)

    def action_resource(self, resource_name):
        self.core_manager.request(COMMAND.VIEW_TEXTURE, resource_name)

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            meta_data = resource.meta_data
            if self.is_new_external_data(meta_data, meta_data.source_filepath):
                self.convert_resource(resource, meta_data.source_filepath)

            texture_datas = self.load_resource_data(resource)
            if texture_datas:
                texture_type = texture_datas.get('texture_type')
                if TextureCube == texture_type or TextureCube.__name__ == texture_type:
                    default_texture = self.resource_manager.get_default_texture()
                    texture_datas['texture_positive_x'] = self.get_resource_data(
                        texture_datas['texture_positive_x']) or default_texture
                    texture_datas['texture_negative_x'] = self.get_resource_data(
                        texture_datas['texture_negative_x']) or default_texture
                    texture_datas['texture_positive_y'] = self.get_resource_data(
                        texture_datas['texture_positive_y']) or default_texture
                    texture_datas['texture_negative_y'] = self.get_resource_data(
                        texture_datas['texture_negative_y']) or default_texture
                    texture_datas['texture_positive_z'] = self.get_resource_data(
                        texture_datas['texture_positive_z']) or default_texture
                    texture_datas['texture_negative_z'] = self.get_resource_data(
                        texture_datas['texture_negative_z']) or default_texture

                texture = CreateTexture(name=resource.name, **texture_datas)
                resource.set_data(texture)
                return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def generate_cube_textures(self):
        cube_faces = ('right', 'left', 'top', 'bottom', 'back', 'front')
        cube_texutre_map = dict()  # { cube_name : { face : source_filepath } }

        # gather cube texture names
        for resource_name in self.resources:
            if '_' in resource_name:
                index = resource_name.rfind('_')
                texture_name = resource_name[:index]
                texutre_face = resource_name[index + 1:]
                if texutre_face in cube_faces:
                    if texture_name not in cube_texutre_map:
                        cube_texutre_map[texture_name] = dict()
                    cube_texutre_map[texture_name][texutre_face] = self.resources[resource_name]

        for cube_texture_name in cube_texutre_map:
            cube_faces = cube_texutre_map[cube_texture_name]
            if len(cube_faces) == 6:
                isCreateCube = any([cube_face in self.new_texture_list for cube_face in cube_faces])
                cube_resource = self.get_resource(cube_texture_name, noWarn=True)
                if cube_resource is None:
                    cube_resource = self.create_resource(cube_texture_name)
                    isCreateCube = True

                if isCreateCube:
                    default_texture = self.get_resource_data('common.flat_gray')
                    texture_right = cube_faces['right'].get_data() or default_texture
                    texture_left = cube_faces['left'].get_data() or default_texture
                    texture_top = cube_faces['top'].get_data() or default_texture
                    texture_bottom = cube_faces['bottom'].get_data() or default_texture
                    texture_back = cube_faces['back'].get_data() or default_texture
                    texture_front = cube_faces['front'].get_data() or default_texture

                    cube_texture_datas = copy.copy(texture_front.__dict__)
                    cube_texture_datas['name'] = cube_texture_name
                    cube_texture_datas['texture_type'] = TextureCube
                    cube_texture_datas['texture_positive_x'] = texture_right
                    cube_texture_datas['texture_negative_x'] = texture_left
                    cube_texture_datas['texture_positive_y'] = texture_top
                    cube_texture_datas['texture_negative_y'] = texture_bottom
                    cube_texture_datas['texture_positive_z'] = texture_front
                    cube_texture_datas['texture_negative_z'] = texture_back

                    cube_texture = CreateTexture(**cube_texture_datas)
                    cube_resource.set_data(cube_texture)
                    cube_texture_datas = cube_texture.get_save_data()
                    self.save_resource_data(cube_resource, cube_texture_datas, '')
        self.new_texture_list = []

    @staticmethod
    def create_texture_from_file(texture_name, source_filepath):
        if os.path.exists(source_filepath):
            image = Image.open(source_filepath)
            width, height = image.size

            # check size is power of two.
            use_power_of_2 = False
            if use_power_of_2:
                width2 = (2 ** math.ceil(math.log2(width))) if 4 < width else 4
                height2 = (2 ** math.ceil(math.log2(height))) if 4 < width else 4
                if width != width2 or height != height2:
                    logger.info('Image Resized (%s) -> (%s) : %s' % ((width, height), (width2, height2), source_filepath))
                    image = image.resize((width2, height2), Image.ANTIALIAS)
                    width, height = width2, height2

            if image.mode == 'L' or image.mode == 'LA' or image.mode == 'P' or image.mode == 'R':
                rgbimg = Image.new("RGBA", image.size)
                rgbimg.paste(image)
                image = rgbimg
                logger.info('Convert Grayscale image to RGB : %s' % source_filepath)

            data = image.tobytes("raw", image.mode, 0, -1)

            texture_datas = dict(
                texture_type=Texture2D,
                image_mode=image.mode,
                width=width,
                height=height,
                data=data
            )
            return CreateTexture(name=texture_name, **texture_datas)
        return None

    def convert_resource(self, resource, source_filepath):
        try:
            logger.info("Convert Resource : %s" % source_filepath)
            if resource not in self.new_texture_list:
                self.new_texture_list.append(resource)

            texture = self.create_texture_from_file(resource.name, source_filepath)
            if texture:
                resource.set_data(texture)
                texture_datas = texture.get_save_data()
                self.save_resource_data(resource, texture_datas, source_filepath)
        except:
            logger.error(traceback.format_exc())
        logger.info("Failed to convert resource : %s" % source_filepath)


# -----------------------#
# CLASS : ProceduralTextureLoader
# -----------------------#
class ProceduralTextureLoader(ResourceLoader):
    name = "ProceduralTextureLoader"
    resource_dir_name = 'ProceduralTextures'
    resource_type_name = 'ProceduralTexture'
    resource_version = 0
    USE_FILE_COMPRESS_TO_SAVE = False
    enable_basic_mode = False
    fileExt = '.ptexture'

    def initialize(self):
        # load and regist resource
        super(ProceduralTextureLoader, self).initialize()

        def create_procedural_texture(resource_name, resource_type):
            if not self.hasResource(resource_name):
                self.create_resource(resource_name, resource_type())
                self.save_resource(resource_name)

        create_procedural_texture("CloudTexture3D", CloudTexture3D)
        create_procedural_texture("VectorFieldTexture3D", VectorFieldTexture3D)
        create_procedural_texture("NoiseTexture3D", NoiseTexture3D)

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            data = self.load_resource_data(resource)
            if data is not None:
                resource_data = CreateProceduralTexture(**data)
                resource.set_data(resource_data)
                return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def action_resource(self, resource_name):
        texture = self.get_resource_data(resource_name)
        if texture is not None:
            texture.generate_texture()


# -----------------------#
# CLASS : MeshLoader
# -----------------------#
class MeshLoader(ResourceLoader):
    name = "MeshLoader"
    resource_version = 0
    resource_dir_name = 'Meshes'
    resource_type_name = 'Mesh'
    fileExt = '.mesh'
    externalFileExt = dict(WaveFront='.obj', Collada='.dae')
    USE_FILE_COMPRESS_TO_SAVE = True

    def initialize(self):
        # load and regist resource
        super(MeshLoader, self).initialize()

        # Regist basic meshs
        self.create_resource("Triangle", Triangle("Triangle"))
        self.create_resource("Quad", Quad("Quad"))
        self.create_resource("Cube", Cube("Cube"))
        self.create_resource("Plane", Plane("Plane", width=4, height=4, xz_plane=True))

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            mesh_data = self.load_resource_data(resource)
            if mesh_data:
                mesh = Mesh(resource.name, **mesh_data)
                resource.set_data(mesh)
                return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def convert_resource(self, resoure, source_filepath):
        logger.info("Convert Resource : %s" % source_filepath)
        file_ext = os.path.splitext(source_filepath)[1].lower()
        if file_ext == self.externalFileExt.get('WaveFront'):
            mesh = OBJ(source_filepath, 1, True)
            mesh_data = mesh.get_mesh_data()
        elif file_ext == self.externalFileExt.get('Collada'):
            mesh = Collada(source_filepath)
            mesh_data = mesh.get_mesh_data()
        else:
            return

        if mesh_data:
            # create mesh
            mesh = Mesh(resoure.name, **mesh_data)
            resoure.set_data(mesh)
            self.save_resource_data(resoure, mesh_data, source_filepath)

    def action_resource(self, resource_name):
        mesh = self.get_resource_data(resource_name)
        if mesh:
            self.resource_manager.model_loader.create_model(mesh)


# -----------------------#
# CLASS : ModelLoader
# -----------------------#
class ModelLoader(ResourceLoader):
    name = "ModelLoader"
    resource_dir_name = 'Models'
    resource_type_name = 'Model'
    fileExt = '.model'
    externalFileExt = dict(Mesh='.mesh')
    USE_FILE_COMPRESS_TO_SAVE = False

    def initialize(self):
        # load and regist resource
        super(ModelLoader, self).initialize()

        # Regist basic meshs
        for mesh_name in ("Triangle", "Quad", "Cube"):
            self.create_resource(mesh_name, Model(mesh_name, mesh=self.resource_manager.get_mesh(mesh_name)))

    def create_model(self, mesh):
        resource = self.create_resource(mesh.name)
        model = Model(resource.name, mesh=mesh)
        resource.set_data(model)
        self.save_resource(resource.name)

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            object_data = self.load_resource_data(resource)
            if object_data:
                mesh = self.resource_manager.get_mesh(object_data.get('mesh'))
                material_instances = [self.resource_manager.get_material_instance(material_instance_name)
                                      for material_instance_name in object_data.get('material_instances', [])]
                obj = Model(resource.name, mesh=mesh, material_instances=material_instances)
                resource.set_data(obj)
                return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def action_resource(self, resource_name):
        model = self.get_resource_data(resource_name)
        if model:
            self.scene_manager.add_object_here(model)


# -----------------------#
# CLASS : SplineLoader
# -----------------------#
class SplineLoader(ResourceLoader):
    name = "SplineLoader"
    resource_dir_name = 'Splines'
    resource_type_name = 'Spline'
    fileExt = '.spline'
    externalFileExt = dict(Mesh='.spline')
    USE_FILE_COMPRESS_TO_SAVE = False

    def initialize(self):
        # load and regist resource
        super(SplineLoader, self).initialize()

        self.create_resource("default_spline", SplineData("default_spline"))

    def create_spline(self):
        resource = self.create_resource('spline')
        spline = SplineData("spline")
        resource.set_data(spline)
        self.save_resource(resource.name)

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            spline_data = self.load_resource_data(resource)
            if spline_data:
                spline_points = spline_data.get('spline_points', [])
                for i, spline_point in enumerate(spline_points):
                    spline_points[i] = SplinePoint(Float3(*spline_point['position']), Float3(*spline_point['control_point']), spline_point.get('point_time', 1.0))
                spline_data['spline_points'] = spline_points
                spline_data['name'] = resource_name
                spline = SplineData(**spline_data)
                resource.set_data(spline)
                return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def action_resource(self, resource_name):
        self.scene_manager.add_spline_here(name=resource_name, spline_data=resource_name)


# -----------------------#
# CLASS : SceneLoader
# -----------------------#
class SceneLoader(ResourceLoader):
    name = "SceneLoader"
    resource_dir_name = 'Scenes'
    resource_type_name = 'Scene'
    fileExt = '.scene'
    USE_FILE_COMPRESS_TO_SAVE = False

    def save_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource and resource_name == self.scene_manager.get_current_scene_name():
            scene_data = self.scene_manager.get_save_data()
            self.save_resource_data(resource, scene_data)

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            meta_data = self.get_meta_data(resource_name)
            if resource and meta_data:
                if os.path.exists(meta_data.resource_filepath):
                    scene_datas = self.load_resource_data(resource)
                else:
                    scene_datas = resource.get_data()

                if scene_datas:
                    for object_data in scene_datas.get('static_actors', []):
                        object_data['model'] = self.resource_manager.get_model(object_data.get('model'))

                    for object_data in scene_datas.get('skeleton_actors', []):
                        object_data['model'] = self.resource_manager.get_model(object_data.get('model'))

                    self.scene_manager.open_scene(resource_name, scene_datas)
                    resource.set_data(scene_datas)
                    return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def action_resource(self, resource_name):
        self.load_resource(resource_name)


# -----------------------#
# CLASS : FontLoader
# -----------------------#
class FontLoader(ResourceLoader):
    """
    http://jrgraphix.net/research/unicode.php
    """
    name = "FontLoader"
    resource_dir_name = 'Fonts'
    resource_type_name = 'Font'
    resource_version = 2
    fileExt = '.font'
    externalFileExt = dict(TTF='.ttf', OTF='.otf')
    enable_basic_mode = False

    unicode_blocks = dict(
        Basic_Latin=(0x20, 0x7F),  # 32 ~ 127
        Hangul_Syllables=(0xAC00, 0xD7AF),  # 44032 ~ 55215
    )

    def check_font_data(self, font_datas, resoure, source_filepath):
        chaneged = False

        if self.is_engine_resource(source_filepath):
            preview_path = self.engine_resource_path
        else:
            preview_path = self.project_resource_path

        for unicode_block_name in self.unicode_blocks:
            if unicode_block_name not in font_datas:
                range_min, range_max = self.unicode_blocks[unicode_block_name]
                font_data = generate_font_data(
                    resource_name=resoure.name,
                    distance_field_font=False,
                    anti_aliasing=True,
                    font_size=20,
                    padding=1,
                    unicode_block_name=unicode_block_name,
                    range_min=range_min,
                    range_max=range_max,
                    source_filepath=source_filepath,
                    preview_path=preview_path
                )
                font_datas[unicode_block_name] = font_data
                chaneged = True

        if font_datas is not None and chaneged:
            self.save_resource_data(resoure, font_datas, source_filepath)
        return font_datas

    def convert_resource(self, resoure, source_filepath):
        logger.info("Convert Resource : %s" % source_filepath)
        font_datas = {}
        self.check_font_data(font_datas, resoure, source_filepath)

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            meta_data = resource.meta_data
            font_datas = self.load_resource_data(resource)
            if font_datas is not None:
                font_datas = self.check_font_data(font_datas, resource, meta_data.source_filepath)

                for unicode_block_name in font_datas:
                    font_data = font_datas[unicode_block_name]

                    if font_data is not None:
                        texture_datas = dict(
                            texture_type=Texture2D,
                            image_mode=font_data.get('image_mode'),
                            width=font_data.get('image_width'),
                            height=font_data.get('image_height'),
                            data=font_data.get('image_data'),
                            min_filter=GL_LINEAR,
                            mag_filter=GL_LINEAR,
                        )
                        texture_name = "_".join([resource_name, font_data.get('unicode_block_name')])
                        font_data['texture'] = CreateTexture(name=texture_name, **texture_datas)
                        font_datas[unicode_block_name] = FontData(unicode_block_name, font_data)

                resource.set_data(font_datas)
                return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False


# -----------------------#
# CLASS : EffectLoader
# -----------------------#
class EffectLoader(ResourceLoader):
    name = "EffectLoader"
    resource_dir_name = 'Effects'
    resource_type_name = 'Effect'
    fileExt = '.effect'
    USE_FILE_COMPRESS_TO_SAVE = False
    enable_basic_mode = False

    def create_effect(self, particle_info=None):
        resource = self.create_resource('effect')
        if particle_info is not None:
            particle_infos = [particle_info, ]
        else:
            particle_infos = []
        effect = EffectInfo(resource.name, particle_infos=particle_infos)
        resource.set_data(effect)
        self.save_resource(resource.name)

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            effect_info = self.load_resource_data(resource)
            if effect_info is not None:
                particle_infos = []
                for particle_name in effect_info.get('particle_infos', []):
                    particle_info = self.resource_manager.get_particle(particle_name)
                    particle_infos.append(particle_info)
                effect_info['particle_infos'] = particle_infos
                effect_info = EffectInfo(resource_name, **effect_info)
                resource.set_data(effect_info)
                return True
        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def action_resource(self, resource_name):
        self.scene_manager.add_effect_here(name=resource_name, effect_info=resource_name)


# -----------------------#
# CLASS : ParticleLoader
# -----------------------#
class ParticleLoader(ResourceLoader):
    name = "ParticleLoader"
    resource_dir_name = 'Effects'
    resource_type_name = 'Particle'
    fileExt = '.particle'
    USE_FILE_COMPRESS_TO_SAVE = False
    enable_basic_mode = False

    def create_particle(self):
        resource = self.create_resource('particle')
        effect = ParticleInfo(resource.name)
        resource.set_data(effect)
        self.save_resource(resource.name)

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            particle_info = self.load_resource_data(resource)
            particle_info['mesh'] = self.resource_manager.get_mesh(particle_info.get('mesh'))
            particle_info['material_instance'] = self.resource_manager.get_material_instance(particle_info.get('material_instance'))
            particle_info['texture_diffuse'] = self.resource_manager.get_texture(particle_info.get('texture_diffuse'))
            particle_info = ParticleInfo(resource_name, **particle_info)
            resource.set_data(particle_info)
            return True

        logger.error('%s failed to load %s' % (self.name, resource_name))
        return False

    def action_resource(self, resource_name):
        particle_info = self.get_resource_data(resource_name)
        if particle_info:
            self.resource_manager.effect_loader.create_effect(particle_info)


# -----------------------#
# CLASS : SoundLoader
# -----------------------#
class SoundLoader(ResourceLoader):
    name = "SoundLoader"
    resource_dir_name = 'Sounds'
    resource_type_name = 'Sound'
    fileExt = '.wav'
    externalFileExt = dict(Sound='.wav')
    USE_FILE_COMPRESS_TO_SAVE = False

    def clear(self):
        for resource_name in self.resources:
            resource = self.resources[resource_name]
            if resource.data is not None:
                resource.data.destroy()
                resource.data = None

    def close(self):
        self.clear()
        self.resources.clear()

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            try:
                sound = self.sound_manager.create_sound(resource.meta_data.resource_filepath)
                resource.set_data(sound)
                return True
            except:
                logger.error(traceback.format_exc())
                pass
        return False

    def action_resource(self, resource_name):
        self.sound_manager.play_sound(resource_name)


# -----------------------#
# CLASS : ScriptLoader
# -----------------------#
class ScriptLoader(ResourceLoader):
    name = "ScriptLoader"
    resource_dir_name = 'Scripts'
    resource_type_name = 'Script'
    fileExt = '.py'
    USE_FILE_COMPRESS_TO_SAVE = False

    def load_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            try:
                module = SourceFileLoader("module.name", resource.meta_data.resource_filepath).load_module()
                resource.set_data(module)
                return True
            except:
                logger.error(traceback.format_exc())
                pass
        return False

    def unload_resource(self, resource_name):
        resource = self.get_resource(resource_name)
        if resource:
            resource.clear_data()
            return True
        return False

    def reload(self):
        for resource_name in self.resources:
            self.unload_resource(resource_name)

        for resource in self.resources.values():
            module = SourceFileLoader("module.name", resource.meta_data.resource_filepath).load_module()
            resource.set_data(module)

            loaded_modules = {}
            script_path = self.project_resource_path

            # recursive reload sub modules
            def reload_module(module):
                for attribute_name in dir(module):
                    sub_module = getattr(module, attribute_name)

                    if types.ModuleType is type(sub_module) and hasattr(sub_module, "__file__") and sub_module.__file__.startswith(script_path):
                        if sub_module.__file__ in loaded_modules:
                            continue
                        else:
                            loaded_modules[sub_module.__file__] = ()
                        logger.info("load : %s" % sub_module.__file__)
                        importlib.reload(sub_module)
                        reload_module(sub_module)
            reload_module(module)


# -----------------------#
# CLASS : ResourceManager
# -----------------------#
class ResourceManager(Singleton):
    name = "ResourceManager"
    engine_path = "Resource"
    DefaultProjectFile = os.path.join(engine_path, "default.project")

    def __init__(self):
        self.project_path = ""
        self.resource_loaders = []
        self.core_manager = None
        self.scene_manager = None
        self.sound_manager = None
        self.font_loader = None
        self.texture_loader = None
        self.shader_loader = None
        self.material_loader = None
        self.material_instance_loader = None
        self.mesh_loader = None
        self.scene_loader = None
        self.effect_loader = None
        self.particle_loader = None
        self.sound_loader = None
        self.script_loader = None
        self.model_loader = None
        self.procedural_texture_loader = None
        # self.loading_thread = LoadingThread(self)

    def regist_loader(self, resource_loader_class):
        resource_loader = resource_loader_class(self)
        if not self.core_manager.is_basic_mode or resource_loader.enable_basic_mode:
            self.resource_loaders.append(resource_loader)
        return resource_loader

    def initialize(self, core_manager, project_path=""):
        self.core_manager = core_manager
        self.scene_manager = core_manager.scene_manager
        self.sound_manager = self.core_manager.sound_manager

        check_directory_and_mkdir(self.engine_path)

        self.project_path = project_path or self.engine_path
        check_directory_and_mkdir(self.project_path)

        # NOTE : Script only load from project path.
        sys.path.append(os.path.join(self.project_path, ScriptLoader.resource_dir_name))

        # Be careful with the initialization order.
        self.font_loader = self.regist_loader(FontLoader)
        self.texture_loader = self.regist_loader(TextureLoader)
        self.shader_loader = self.regist_loader(ShaderLoader)
        self.material_loader = self.regist_loader(MaterialLoader)
        self.material_instance_loader = self.regist_loader(MaterialInstanceLoader)
        self.mesh_loader = self.regist_loader(MeshLoader)
        self.spline_loader = self.regist_loader(SplineLoader)
        self.scene_loader = self.regist_loader(SceneLoader)
        self.effect_loader = self.regist_loader(EffectLoader)
        self.particle_loader = self.regist_loader(ParticleLoader)
        self.sound_loader = self.regist_loader(SoundLoader)
        self.script_loader = self.regist_loader(ScriptLoader)
        self.model_loader = self.regist_loader(ModelLoader)
        self.procedural_texture_loader = self.regist_loader(ProceduralTextureLoader)

        # start loading thread
        # self.loading_thread.start()

        # initialize
        for resource_loader in self.resource_loaders:
            if not self.core_manager.is_basic_mode or resource_loader.enable_basic_mode:
                resource_loader.initialize()

        logger.info("Resource register done.")

    def update(self):
        pass
        # self.loading_thread.set_data()

    def close(self):
        for resource_loader in self.resource_loaders:
            if not self.core_manager.is_basic_mode or resource_loader.enable_basic_mode:
                resource_loader.close()

    def prepare_project_directory(self, new_project_dir):
        check_directory_and_mkdir(new_project_dir)
        # copy_tree(self.engine_path, new_project_dir)
        # default_project_file = os.path.join(new_project_dir, 'defualt.project')
        # if os.path.exists(default_project_file):
        #     os.remove(default_project_file)

    def get_resource_name_and_type_list(self):
        """
        :return [(resource name, resource type)]:
        """
        result = []
        for resource_loader in self.resource_loaders:
            result += [(resName, resource_loader.resource_type_name) for resName in
                       resource_loader.get_resource_name_list()]
        return

    def set_resource_attribute(self, resource_name, resource_type_name, attribute_name, attribute_value,
                               parent_info, attribute_index):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            return resource_loader.set_resource_attribute(resource_name, attribute_name, attribute_value,
                                                          parent_info, attribute_index)
        return None

    def get_resource_attribute(self, resource_name, resource_type_name):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            return resource_loader.get_resource_attribute(resource_name)
        return None

    def get_resource_data(self, resource_name, resource_type_name):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            return resource_loader.get_resource_data(resource_name)
        return None

    def add_resource_component(self, resource_name, resource_type_name, attribute_name, parent_info, attribute_index):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            return resource_loader.add_resource_component(resource_name, attribute_name, parent_info, attribute_index)
        return None

    def delete_resource_component(self, resource_name, resource_type_name, attribute_name, parent_info, attribute_index):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            return resource_loader.delete_resource_component(resource_name, attribute_name, parent_info, attribute_index)
        return None

    def get_meta_data(self, resource_name, resource_type_name):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            return resource_loader.get_meta_data(resource_name)
        return None

    def load_resource(self, resource_name, resource_type_name):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            resource_loader.load_resource(resource_name)

    def action_resource(self, resource_name, resource_type_name):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            resource_loader.action_resource(resource_name)

    def duplicate_resource(self, resource_name, resource_type_name):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            resource_loader.duplicate_resource(resource_name)

    def save_resource(self, resource_name, resource_type_name):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            resource_loader.save_resource(resource_name)

    def rename_resource(self, resource_name, resource_type_name):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            resource_loader.rename_resource(resource_name)

    def delete_resource(self, resource_name, resource_type_name):
        resource_loader = self.find_resource_loader(resource_type_name)
        if resource_loader:
            resource_loader.delete_resource(resource_name)

    def find_resource_loader(self, resource_type_name):
        for resource_loader in self.resource_loaders:
            if resource_loader.resource_type_name == resource_type_name:
                return resource_loader
        logger.error("%s is a unknown resource type." % resource_type_name)
        return None

    def open_scene(self, scene_name, force=False):
        if (scene_name != self.scene_manager.get_current_scene_name()) or force:
            self.scene_loader.load_resource(scene_name)

    # FUNCTIONS : Font
    def get_font_data(self, font_name, unicode_block_name):
        font_data = self.font_loader.get_resource_data(font_name)
        return font_data.get(unicode_block_name) if font_data is not None else None

    def get_default_font_data(self, unicode_block_name='Basic_Latin'):
        return self.get_font_data('NanumBarunGothic', unicode_block_name)

    # FUNCTIONS : Shader
    def get_shader_version(self):
        return self.shader_loader.get_shader_version()

    def get_shader(self, shader_name):
        return self.shader_loader.get_resource_data(shader_name)

    def get_material(self, shader_name, macros={}):
        return self.material_loader.get_material(shader_name, macros)

    def get_default_material_instance(self, skeletal=False):
        if skeletal:
            return self.material_instance_loader.get_material_instance(name='default_skeletal',
                                                                       shader_name='default',
                                                                       macros={'SKELETAL': 1})
        return self.material_instance_loader.get_material_instance('default')

    def get_material_instance(self, name, shader_name='', macros={}):
        return self.material_instance_loader.get_material_instance(name,
                                                                   shader_name=shader_name,
                                                                   macros=macros) or \
               self.get_default_material_instance(skeletal=(True if 1 == macros.get('SKELETAL', 0) else 0))

    def get_default_effect_material_instance(self):
        return self.material_instance_loader.get_material_instance('effect.particle_ps')

    def get_default_mesh(self):
        return self.get_mesh('Quad')

    def get_mesh(self, mesh_name):
        return self.mesh_loader.get_resource_data(mesh_name) or self.get_default_mesh()

    def get_default_spline(self):
        return self.get_spline('default_spline')

    def get_spline(self, spline_name):
        return self.spline_loader.get_resource_data(spline_name) or self.get_default_spline()

    def get_procedural_texture(self, texture_name):
        return self.procedural_texture_loader.get_resource_data(texture_name)

    def get_default_texture(self):
        return self.texture_loader.get_resource_data('common.flat_white')

    def get_texture(self, texture_name, default_texture=True):
        if default_texture:
            return self.texture_loader.get_resource_data(texture_name) or self.get_default_texture()
        return self.texture_loader.get_resource_data(texture_name)

    def get_texture_or_none(self, texture_name):
        return self.texture_loader.get_resource_data(texture_name)

    def get_model(self, model_name):
        return self.model_loader.get_resource_data(model_name)

    def get_scene(self, scene_name):
        return self.scene_loader.get_resource_data(scene_name)

    def get_default_effect(self):
        return self.effect_loader.get_resource_data('default_effect')

    def get_effect(self, effect_name):
        return self.effect_loader.get_resource_data(effect_name) or self.get_default_effect()

    def get_default_particle(self):
        return self.particle_loader.get_resource_data('default_particle')

    def get_particle(self, particle_name):
        return self.particle_loader.get_resource_data(particle_name) or self.get_default_particle()

    def get_script(self, script_name):
        return self.script_loader.get_resource_data(script_name)

    def get_sound(self, sound_name):
        return self.sound_loader.get_resource_data(sound_name)

    def get_file_path(self, file_path=""):
        return os.path.join(self.project_path, file_path)
