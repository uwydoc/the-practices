#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Image resource
#

import os
import time
import uuid

import falcon
import msgpack

# Helper methods
def _media_type_to_ext(media_type):
    # strip the 'image/' part off
    return media_type[6:]

def _ext_to_media_type(ext):
    return 'image/' + ext

def _generate_id():
    return str(uuid.uuid4())


class Collection(object):

    def __init__(self, storage_path):
        self.storage_path = storage_path

    def on_post(self, req, resp):
        image_id = _generate_id()
        ext = _media_type_to_ext(req.content_type)
        filename = image_id + '.' + ext

        image_path = os.path.join(self.storage_path, filename)

        with open(image_path, 'wb') as f:
            while True:
                chunck = req.stream.read(4096)
                if not chunck:
                    break
                f.write(chunck)

        resp.status = falcon.HTTP_201
        resp.location = '/images/' + filename


class Item(object):

    def __init__(self, storage_path):
        self.storage_path = storage_path

    def on_get(self, req, resp, filename):
        ext = os.path.splitext(filename)[1][1:]
        resp.content_type = _ext_to_media_type(ext)

        image_path = os.path.join(self.storage_path, filename)
        resp.stream = open(image_path, 'rb')
        resp.stream_len = os.path.getsize(image_path)

