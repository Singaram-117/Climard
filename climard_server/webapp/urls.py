from django.urls import path
from .views import *

urlpatterns = [
    path('', home, name='home'),
    path('led/', led, name='led'),
    path('register_esp32/', register_esp32, name='register_esp32'),
    path('get_esp32_ip/', get_esp32_ip, name='get_esp32_ip'),
    path('endpoint/', receive_data, name='receive_data'),
    path('get_sensor_data/', get_sensor_data, name='get_sensor_data'),
    path('admin_login/', custom_admin_login, name='admin_login'),
    path('change_thresholds/', change_thresholds, name='change_thresholds'),
    path('api/get_thresholds/', get_thresholds, name='get_thresholds'),
]
