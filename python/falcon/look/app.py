#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function, division

import os

import falcon

import images

api = application = falcon.API()

storage_path = os.path.join(os.path.dirname(__file__), 'upload')
if not os.path.exists(storage_path):
    os.makedirs(storage_path, mode=0755)

image_collection = images.Collection(storage_path)
image = images.Item(storage_path)
api.add_route('/images', image_collection)
api.add_route('/images/{filename}', image)
