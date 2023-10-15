from django.urls import path

from . import views

app_name = 'Transactions'
urlpatterns = [
    path('', views.index, name = 'index'),
    path('<int:transaction_id>/', views.detail, name = 'detail'),
    path('new/', views.add_manual, name = 'manual_add'),
    path('list/', views.list, name = 'list'),
    path('upload/', views.read_from_csv, name = 'upload'),
    path('budgets/', views.budget_overview, name = 'budgets'),
    path('budget_add/', views.budget_add, name = 'budget_add'),
    path('budget_detail/<int:budget_id>', views.budget_detail, name = 'budget_detail')
]
