#!/usr/bin/env python
__author__ = 'conghu'
    #!/usr/bin/python
    #
    # author: ajs
    # license: bsd
    # copyright: re2


import json
import urllib2
import httplib
import datetime
import ssl
jenkinsUrl = "https://ci.moaisdk.com/job/"





def SetLight(id,bri,hue):
    page_index = {"on":True,"bri":bri,"hue":hue}

    day = datetime.datetime.today().weekday()
    t = datetime.datetime.now().hour
    if(day>4 or t > 22 or t < 8):
        page_index = {"on":False}

    url = "/api/newdeveloper/lights/{}/state".format(id)

    connection =  httplib.HTTPConnection("192.168.1.3")
    body_content = json.dumps(page_index)
    connection.request('PUT', url, body_content)
    result = connection.getresponse()


def UpdateJobStatus(job,id):
    url =  job + "/lastBuild/api/json"

    gcontext = ssl.SSLContext(ssl.PROTOCOL_TLSv1)
    result = urllib2.urlopen(jenkinsUrl+url, context=gcontext )


    buildStatusJson = json.load( result )
    if buildStatusJson["building"] == True :
        SetLight(id,0,12750)
        return
    if buildStatusJson.has_key( "result" ):
        if buildStatusJson["result"] == "SUCCESS" :
            SetLight(id,0,25717)
            return
        if buildStatusJson["result"] == 'FAILURE':
            SetLight(id,255,0)
            return
        if buildStatusJson["result"] == 'ABORTED':
            SetLight(id,0,46920)
            return


UpdateJobStatus('FLOW_Moai_dev_develop',1)
UpdateJobStatus('FLOW_Moai_dev_develop_pointinside',2)
UpdateJobStatus('FLOW_Moai_dev_develop_plumzi',3)

