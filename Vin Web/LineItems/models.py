from django.db import models

class LineItem(models.Model):
    amount = models.FloatField()
    sign = models.CharField( max_length = 1)
    currency = models.CharField (max_length = 3)
    date = models.DateField()
    account = models.CharField (max_length = 16)
    comment = models.CharField (max_length = 200)

    def __str__(self):
        return self.sign + str(self.amount) + ' ' + self.currency + ' ' + self.account