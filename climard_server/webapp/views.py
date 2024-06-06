from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import requests

ESP32_IP = None

@csrf_exempt
def register_esp32(request):
    global ESP32_IP
    if request.method == 'POST':
        ESP32_IP = request.POST.get('ip')
        return JsonResponse({'status': 'IP registered', 'ip': ESP32_IP})
    return JsonResponse({'status': 'failed'})

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
