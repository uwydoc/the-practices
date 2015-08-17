#!/usr/bin/env python
#
# things.py
#

import falcon

class ThingsResource(object):
    def on_get(self, req, resp):
        '''Handles GET requests'''
        resp.status = falcon.HTTP_200
        resp.body = ('\nTwo things awe me most, the starry sky '
                     'above me and the moral law within me.\n'
                     '\n'
                     '     ~ immanuel Kant\n\n')

app = falcon.API()

things = ThingsResource()

app.add_route('/things', things)
