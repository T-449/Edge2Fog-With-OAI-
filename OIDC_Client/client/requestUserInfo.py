import requests

tokenFile = open('tokens.txt', 'r')
token = tokenFile.readline()
getRequest = 'http://localhost:8000/userinfo/?access_token=' + token
userInfo = requests.get(getRequest)
print(userInfo.text)