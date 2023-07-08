# この django.shortcuts は、よく使う構文をより簡潔にかけるように用意されたもの
from django.shortcuts import render, get_object_or_404
from django.http import Http404

# Create your views here.
from django.http import HttpResponse
from django.template import loader
from .models import Question

def index(request):
    latest_question_list = Question.objects.order_by("-pub_date")[:5]
    context = {"latest_question_list": latest_question_list}
    return render(request, "polls/index.html", context)

def detail(request, question_id):
    question = get_object_or_404(Question, pk=question_id)
    return render(request, "polls/detail.html", {"question": question})

def results(request, question_id):
    response = "You're looking at the results of question %s."
    return HttpResponse(response % question_id)

def vote(request, question_id):
    return HttpResponse("You're voting on question %s." % question_id)


"""
# [view関数をひな形を使わずに記述する方法]
# 素直な書き方
# テンプレートをロードしてコンテキストに値を入れ、
# テンプレートをレンダリングした結果を HttpResponse オブジェクトで返す
def index(request):
    template = loader.get_template("polls/index.html")
    latest_question_list = Question.objects.order_by("-pub_date")[:5]
    # polls/index.html 内のテンプレート変数を Python オブジェクトにマッピングする辞書
    context = {
        "latest_question_list": latest_question_list,
    }
    return HttpResponse(template.render(context, request))

# ショートカット render でカンタンに書く方法      ← 基本 この方法使えばよさそう
# loader, HttpResponse を省略可能！
# render関数はテンプレートを指定のコンテキストでレンダリングし、その HttpResponse オブジェクトを返す
# 第3引数contextは任意(テンプレート変数がない場合は当然不要だから)
def index(request):
    latest_question_list = Question.objects.order_by("-pub_date")[:5]
    context = {"latest_question_list": latest_question_list}
    return render(request, "polls/index.html", context)
"""

"""
# get() を実行し、オブジェクトが存在しない場合には Http404 を送出
# 素直な書き方
def detail(request, question_id):
    try:
        question = Question.objects.get(pk=question_id)
    except Question.DoesNotExist:
        raise Http404("Question does not exist")
    return render(request, "polls/detail.html", {"question": question})

# ショートカット get_object_or_404
# Django モデルを第一引数に、任意の数のキーワード引数を取り、
# モデルのマネージャの get() 関数に渡します。オブジェクトが存在しない場合は Http404 を発生
def detail(request, question_id):
    question = get_object_or_404(Question, pk=question_id)
    return render(request, "polls/detail.html", {"question": question})
"""