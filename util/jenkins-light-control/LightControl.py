#!/usr/bin/env python
__author__ = 'conghu'
import jenkins
import json
import httplib
import datetime
def SetLight(id,bri,hue):
    page_index = {"on":True,"bri":bri,"hue":hue}

    day = datetime.datetime.today().weekday()
    t = datetime.datetime.now().hour
    if(day>4 or t > 22 or t < 8):
        page_index = {"on":False}

    url = "/api/newdeveloper/lights/{}/state".format(id)

    connection =  httplib.HTTPConnection("192.168.1.38")
    body_content = json.dumps(page_index)
    connection.request('PUT', url, body_content)
    result = connection.getresponse()



def UpdateJobStatus(job,id):
    j = jenkins.Jenkins('https://ci.moaisdk.com', 'BuildMonitor', '654321')
    j.get_jobs()
    last_build_number = j.get_job_info(job)["color"]
    if last_build_number == 'blue':
        SetLight(id,0,25717)
        return
    if last_build_number == 'red':
        SetLight(id,255,0)
        return
    if last_build_number == 'aborted':
        SetLight(id,0,46920)
        return
    SetLight(id,0,12750)

UpdateJobStatus('FLOW_Moai_dev_develop',1)
UpdateJobStatus('FLOW_Moai_dev_develop_pointinside',2)
UpdateJobStatus('FLOW_Moai_dev_develop_plumzi',3)
