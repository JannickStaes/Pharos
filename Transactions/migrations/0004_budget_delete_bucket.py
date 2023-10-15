# Generated by Django 4.2.6 on 2023-10-15 11:26

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Transactions', '0003_category_alter_transaction_id_bucket_and_more'),
    ]

    operations = [
        migrations.CreateModel(
            name='Budget',
            fields=[
                ('id', models.BigAutoField(primary_key=True, serialize=False)),
                ('target', models.FloatField()),
                ('actuals', models.FloatField()),
                ('eac', models.FloatField()),
                ('startDate', models.DateField()),
                ('endDate', models.DateField()),
                ('transactions', models.ManyToManyField(to='Transactions.transaction')),
            ],
        ),
        migrations.DeleteModel(
            name='Bucket',
        ),
    ]
