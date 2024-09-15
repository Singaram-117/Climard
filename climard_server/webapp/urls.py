from django.urls import path
from .views import *

urlpatterns = [
    path('', home, name='home'),
    path('endpoint/', receive_data, name='receive_data'),
    path('get_sensor_data/', get_sensor_data, name='get_sensor_data'),
    path('admin_login/', custom_admin_login, name='admin_login'),
    path('change_thresholds/', change_thresholds, name='change_thresholds'),
    path('api/get_thresholds/', get_thresholds, name='get_thresholds'),
]
