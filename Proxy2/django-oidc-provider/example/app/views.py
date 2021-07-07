import os
import django
from django.contrib.auth import login, authenticate, logout
from django.shortcuts import redirect

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'app.settings')
django.setup()


def loginView(request):
    myuserFile = open('myUser.txt', 'r')
    line = myuserFile.readline()
    line = line.split(' ')
    user = authenticate(request, username=line[0], password=line[2])
    login(request, user)
    return redirect(request.GET.get('next'))
