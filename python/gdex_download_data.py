#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# @file gdex_download_data.py
#
# @breif Download tif data from http://gdex.cr.usgs.gov/gdex/
#
from __future__ import division, print_function

import requests
import string

from contextlib import closing

__all__ = [
    'get_download_url'
    , 'download'
]

def get_download_url(x1, y1, x2, y2, format='GeoTIFF', research_area='climate', verbose=False):
    host = 'http://gdex.cr.usgs.gov'
    port = 80
    method = 'POST'
    path = '/gdex/DownloadData'
    headers = {
        'Accept': '*/*'
        , 'Accept-Encoding': 'gzip, deflate'
        , 'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'
        , 'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.124 Safari/537.36'
        , 'X-Request-With': 'XMLHttpRequest'
    }
    form = {
        'seldata': 'ASTERDEM'
        , 'researcharea': 'climate'
    }
    bound_str_template = string.Template('$x1,$y1,$x2,$y2')
    context_xml_template = string.Template('''<wfs:FeatureCollection xmlns:wfs="http://www.opengis.net/wfs"><gml:featureMember xmlns:gml="http://www.opengis.net/gml"><feature:features xmlns:feature="http://mapserver.gis.umn.edu/mapserver" fid="OpenLayers.Feature.Vector_394"><feature:geometry><gml:Polygon><gml:outerBoundaryIs><gml:LinearRing><gml:coordinates decimal="." cs="," ts=" ">$x1,$y1 $x1,$y2 $x2,$y2 $x2,$y1</gml:coordinates></gml:LinearRing></gml:outerBoundaryIs></gml:Polygon></feature:geometry></feature:features></gml:featureMember></wfs:FeatureCollection>''')
    #
    form['bound_str'] = bound_str_template.substitute(x1=x1, y1=y1, x2=x2, y2=y2)
    form['multi_bound_str'] = form['bound_str']
    form['context_xml'] = context_xml_template.substitute(x1=x1, y1=y1, x2=x2, y2=y2)
    form['researcharea'] = research_area
    form['outputformat'] = format
    if format == 'GeoTIFF':
        form['projection'] = 'EPSG:4326'
        form['zipflag'] = 'on'
    elif format == '1x1Tiles':
        pass
    else:
        raise Exception('Unsupported output format - "%s"' % (format))
    if verbose:
        print(headers)
        print(form)
    with closing(requests.request(method, host + path, headers=headers, data=form)) as resp:
        if verbose or resp.status_code != 200:
            print(resp.status_code, resp.reason)
        data = resp.text
        if resp.status_code == 200:
            parsed = {}
            for x in map(lambda x: x.strip().split(':', 1), data[1:-1].split(',')):
                parsed[x[0]] = x[1]
            if parsed['success'] == 'true':
                return parsed['url'][1:-1]
            else:
                print('[error]', data)
        else:
            print('[error]', data)

def download(url, filename):
    with closing(requests.get(url, stream=True)) as resp:
        if resp.status_code != 200:
            return
        print('file size:', resp.headers['content-length'])
        with open(filename, 'wb') as f:
            for chunk in resp.iter_content(chunk_size=1024):
                if chunk:
                    f.write(chunk)
        return True

if __name__ == '__main__':
    p1 = (120.777513, 30.699223)
    p2 = (122.051927,31.885746)
    print('--- request(%s, %s) ---' % (p1, p2))
    url = get_download_url(p1[0], p1[1], p2[0], p2[1], format='1x1Tiles')
    print('url:', url)
    print('--- download "%s" ---' % (url))
    filename = url.split('/')[-1]
    download(url, filename)
    print('--- done(%s) ---' % (filename))
