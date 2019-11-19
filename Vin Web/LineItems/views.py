from django.shortcuts import render
from django.http import HttpResponse
from django.shortcuts import get_object_or_404

from .models import LineItem

def index(request):
    return render(request, 'LineItems/index.html')

def detail(request, transaction_id):
    transaction = get_object_or_404(LineItem, pk = transaction_id)
    context = { 'transaction': transaction }
    return render(request, 'LineItems/detail.html', context)

def add_manual(request):
    return HttpResponse('You can add a new transaction here.')