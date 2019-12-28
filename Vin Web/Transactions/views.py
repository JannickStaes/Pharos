from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import get_object_or_404
from django.urls import reverse
from VinLibrary import read_from_csv_keytrade

from .models import Transaction

def index(request):
    return render(request, 'Transactions/index.html')

def detail(request, transaction_id):
    transaction = get_object_or_404(Transaction, pk = transaction_id)
    context = { 'transaction': transaction }
    return render(request, 'Transactions/detail.html', context)

def add_manual(request):
    if request.method == 'POST':
        t = Transaction(amount = request.POST['Amount'],
                        sign = '-',
                        currency = request.POST['Currency'],
                        date = request.POST['Date'],
                        account = request.POST['Account'],
                        comment = request.POST['Comment'])
        t.save()
        return HttpResponseRedirect(reverse('Transactions:manual_add'))
    return render(request, 'Transactions/manual_add.html')

def list(request):
    transactions = Transaction.objects.all()
    context = {
        'transactions': transactions
    }
    return render(request, 'Transactions/list.html', context)

def read_from_csv(request):
    if request.method == 'POST':
        items = read_from_csv_keytrade('C:\\Users\\staes\\source\\repos\\JannickStaes\\Vin\\Vin Library\\Keytrade Download.csv')

        print(items[0])
        t = Transaction(amount = items[7]['Amount'],
                        sign = items[7]['Sign'],
                        currency = items[7]['Currency'],
                        date = items[7]['Date'],
                        account = items[7]['Account'],
                        comment = items[7]['Comment'])
        t.save()
        return HttpResponseRedirect(reverse('Transactions:upload'))

    return render(request, 'Transactions/csv_upload.html')