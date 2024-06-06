from django.urls import path
from .views import home, register_esp32

urlpatterns = [
    path('', home, name='home'),
    path('register_esp32/', register_esp32, name='register_esp32'),
]
