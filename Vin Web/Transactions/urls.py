from django.urls import path

from . import views

app_name = 'Transactions'
urlpatterns = [
    path('', views.index, name = 'index'),
    path('<int:transaction_id>/', views.detail, name = 'detail'),
    path('new/', views.add_manual, name = 'manual_add'),
    path('list/', views.list, name = 'list')
]
