from django.db import models

class Transaction(models.Model):
    id = models.BigAutoField(primary_key=True)
    amount = models.FloatField()
    sign = models.CharField( max_length = 1)
    currency = models.CharField( max_length = 3)
    date = models.DateField()
    account = models.CharField( max_length = 16)
    comment = models.CharField( max_length = 200)

    def __str__(self):
        return self.sign + str(self.amount) + ' ' + self.currency + ' ' + self.account
    
class Budget(models.Model):
    id = models.BigAutoField(primary_key=True)

    name = models.CharField ( max_length= 40 )
    target = models.FloatField()
    actuals = models.FloatField()
    eac = models.FloatField()
    currency = models.CharField( max_length = 3)

    startDate= models.DateField()
    endDate = models.DateField()

    transactions = models.ManyToManyField(Transaction)

class Category(models.Model):
    id = models.BigAutoField(primary_key=True)

    name = models.CharField ( max_length= 40 )
    target = models.FloatField()
    actuals = models.FloatField()
    eac = models.FloatField()
    currency = models.CharField( max_length = 3)

    budget = models.ForeignKey(Budget, on_delete=models.CASCADE)
