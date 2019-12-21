from VinLibrary import *

#oonderstaande import geeft een error -> wsl doet Django iets speciaals hiervoor in de views.py
from .models import Transaction

list_items = read_from_csv_keytrade('C:\\Users\\staes\\source\\repos\\JannickStaes\\Vin\\Vin Library\\Keytrade Download.csv')
for item in range(1):
        t = Transaction(amount = item['Amount'],
                        sign = item['Sign'],
                        currency = item['Currency'],
                        date = item['Date'],
                        account = item['Account'],
                        comment = item['Comment'])
        t.save()

