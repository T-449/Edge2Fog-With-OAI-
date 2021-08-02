from django.urls import path
from . import views

urlpatterns = [
    path('', views.startClient, name='start'),
    path('callback', views.callbackClient, name='callback'),
    path('userAccount', views.userAccount, name='account'),
]