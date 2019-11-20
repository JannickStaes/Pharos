from django.db import models

class Transaction(models.Model):
    amount = models.FloatField()
    sign = models.CharField( max_length = 1)
    currency = models.CharField (max_length = 3)
    date = models.DateField()
    account = models.CharField (max_length = 16)
    comment = models.CharField (max_length = 200)

    def __init__(self, amount, sign, currency, date, account, comment):
        self.amount = amount
        self.sign = sign
        self.currency = currency
        self.date = date
        self.account = account
        self.comment = comment

    def __str__(self):
        return self.sign + str(self.amount) + ' ' + self.currency + ' ' + self.account