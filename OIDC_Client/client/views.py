from django.shortcuts import render
import requests

# Create your views here.

def startClient(request):
    stateFile = open('state.txt', 'r')
    disabled = stateFile.readline()
    stateFile.close()
    return render(request, 'jsClient.html', {'state': disabled})


def callbackClient(request):
    stateFile = open('state.txt', 'w')
    stateFile.write('disabled')
    stateFile.close()
    return render(request, 'callback.html')


def userAccount(request):
    token = request.POST['tok']
    getRequest = 'http://localhost:8000/userinfo/?access_token=' + token
    userInfo = str((requests.get(getRequest)).text)
    userInfo = userInfo.replace('"', '')
    userInfo = userInfo[1:len(userInfo) - 1]
    userInfo = userInfo.split(', ')
    return render(request, 'userAccount.html', {'username': (userInfo[len(userInfo)-1].split(':'))[1]})
