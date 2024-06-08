from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import requests
import logging

logger = logging.getLogger(__name__)

ESP32_IP = None

@csrf_exempt
def register_esp32(request):
    global ESP32_IP
    if request.method == 'POST':
        ESP32_IP = request.POST.get('ip')
        mac_address = request.POST.get('mac')
        logger.info(f"Received IP: {ESP32_IP}, MAC: {mac_address}")
        return JsonResponse({'status': 'IP registered', 'ip': ESP32_IP, 'mac': mac_address})
    return JsonResponse({'status': 'failed'})

def get_esp32_ip(request):
    global ESP32_IP
    if ESP32_IP:
        return JsonResponse({'status': 'success', 'ip': ESP32_IP})
    return JsonResponse({'status': 'fail', 'message': 'ESP32 IP not registered'})

def home(request):
    global ESP32_IP
    if request.method == 'POST':
        action = request.POST.get('action')
        if ESP32_IP:
            if action == 'led_on':
                response = requests.get(f"http://{ESP32_IP}/led_on")
            elif action == 'led_off':
                response = requests.get(f"http://{ESP32_IP}/led_off")
            return JsonResponse({'status': response.text})
        else:
            return JsonResponse({'status': 'ESP32 IP not registered'})
    return render(request, 'home.html')
