from VinLibrary import *

list_items = read_from_csv_keytrade('C:\\Users\\staes\\source\\repos\\JannickStaes\\Vin\\Vin Library\\Keytrade Download2.csv')
print(list_items[0])
for item in range(1):
        t = Transaction(amount = item['Amount'],
                        sign = item['Sign'],
                        currency = item['Currency'],
                        date = item['Date'],
                        account = item['Account'],
                        comment = item['Comment'])
        t.save()

