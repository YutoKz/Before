from django.shortcuts import render
from django.urls import reverse_lazy
from django.utils import timezone #独自で追加
# Create your views here.
from django.views.generic import TemplateView, CreateView, ListView, DetailView, UpdateView, DeleteView
from diary.models import Diary
from diary.forms import DiaryForm #独自で追加


class IndexView(TemplateView):
    template_name = 'index.html'

class DiaryCreateView(CreateView):
    template_name = 'diary_create.html'
    form_class = DiaryForm
    # 完了後、遷移する先
    success_url = reverse_lazy('diary:diary_create_complete')

class DiaryCreateCompleteView(TemplateView):
    template_name = 'diary_create_complete.html'

class DiaryListView(ListView):
    template_name = 'diary_list.html' 
    model = Diary
    # diary_list.html内で特に定義なく diary_list という変数を使ってるが、
    # おそらくこれは model = Diary から自動的に定義されている　

class DiaryDetailView(DetailView):
    template_name = 'diary_detail.html'
    model = Diary
    # diary_detail.html内で特に定義なく diary という変数を使ってるが、
    # おそらくこれは model = Diary から自動的に定義されている

class DiaryUpdateView(UpdateView):
    template_name = 'diary_update.html'
    model = Diary
    fields = ('date', 'title', 'text',)
    success_url = reverse_lazy('diary:diary_list')

    def form_valid(self, form):
        diary = form.save(commit=False)
        diary.updated_at = timezone.now()
        diary.save()
        return super().form_valid(form)

class DiaryDeleteView(DeleteView):
    template_name = 'diary_delete.html'
    # 削除するデータに対応するモデル
    model = Diary
    success_url = reverse_lazy('diary:diary_list')