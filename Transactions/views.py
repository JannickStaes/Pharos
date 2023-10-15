from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import get_object_or_404
from django.urls import reverse

from .models import Transaction, Budget

def index(request):
    return render(request, 'Transactions/index.html')

def numberGame(request):
    return render(request, 'Transactions/numberGame.html')

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
        #items = read_from_csv_keytrade('C:\\Users\\staes\\source\\repos\\JannickStaes\\Vin\\Vin Library\\Keytrade Download.csv')

        #  for item in items:
        #     t = Transaction(amount = item['Amount'],
        #                     sign = item['Sign'],
        #                     currency = item['Currency'],
        #                     date = item['Date'],
        #                     account = item['Account'],
        #                     comment = item['Comment'])
        #     t.save() 
        return HttpResponseRedirect(reverse('Transactions:upload'))

    return render(request, 'Transactions/csv_upload.html')

def budget_overview(request):
    budgets = Budget.objects.all()
    context = {
        'budgets': budgets
    }
    return render(request, 'Transactions/budget_overview.html', context)

def budget_add(request):
    budgets = Budget.objects.all()
    context = {
        'budgets': budgets
    }
    if request.method == 'POST':
        currency_symbol = request.POST['Currency']
        currency_code = ''
        if currency_symbol== '€':
            currency_code = 'EUR'
        elif currency_symbol == '$':
            currency_code = 'USD'
        elif currency_symbol == '£':
            currency_code = 'GBP'
        b = Budget( name  = request.POST['Name'],
                    target = request.POST['Target'],
                    actuals = 0.0,
                    eac = 0.0,
                    currency = currency_code,
                    startDate = request.POST['Start Date'],
                    endDate = request.POST['End Date'])
        b.save()
        return HttpResponseRedirect(reverse('Transactions:budget_overview'))
    return render(request, 'Transactions/budget_add.html', context)