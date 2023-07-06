from django.contrib import admin
from .models import Diary
# Register your models here.

# Diary モデルに対応したデータを、管理サイトから閲覧できるようにする
admin.site.register(Diary)