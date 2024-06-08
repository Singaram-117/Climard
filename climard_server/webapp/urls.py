from django.urls import path
from .views import *

urlpatterns = [
    path('', home, name='home'),
    path('register_esp32/', register_esp32, name='register_esp32'),
    path('get_esp32_ip/', get_esp32_ip, name='get_esp32_ip'),
]
