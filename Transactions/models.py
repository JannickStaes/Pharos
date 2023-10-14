from django.db import models

class Category(models.Model):
    id = models.BigAutoField(primary_key=True)
    name = models.CharField( max_length = 40 )

    def __str__(self):
        return self.name

class Transaction(models.Model):
    id = models.BigAutoField(primary_key=True)
    amount = models.FloatField()
    sign = models.CharField( max_length = 1)
    currency = models.CharField( max_length = 3)
    date = models.DateField()
    account = models.CharField( max_length = 16)
    comment = models.CharField( max_length = 200)
    category = models.ForeignKey( Category, on_delete=models.CASCADE)

    def __str__(self):
        return self.sign + str(self.amount) + ' ' + self.currency + ' ' + self.accountpy
    
class Bucket(models.Model):
    id = models.BigAutoField(primary_key=True)
    budget = models.FloatField()
    actuals = models.FloatField()
    eac = models.FloatField()
    startDate= models.DateField()
    endDate = models.DateField()

    transactions = models.ManyToManyField(Transaction)


