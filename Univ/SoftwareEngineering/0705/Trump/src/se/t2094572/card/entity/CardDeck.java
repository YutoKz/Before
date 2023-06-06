package se.t2094572.card.entity;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * カードデッキを表すクラス。カードインスタンスを要素とするArrayListを持つ。
 * 
 * @author Yuto Kizawa
 * @version 0.1, 2022-07-09
 * @since JDK1.8
 * 
 */
public class CardDeck {
	/** デッキ */
	private ArrayList<Card> cards = new ArrayList<Card>();
	
	/** 
	 * 空のデッキインスタンスを作る
	 */
	public CardDeck() {
		
	}
	
	/**
	 * フルデッキにする。
	 */
	public void createFullDeck() {
		clear();
		for(int i = 0; i <= 3; i++) {
			for(int j = 1; j <= 13; j++) {
				Card c = new Card(i, j);
				addCard(c);
			}
		}
	}
	
	/**
	 * 空デッキにする。
	 */
	public void clear() {
		cards.clear();
	}
	
	/**
	 * シャッフルする。
	 */
	public void shuffle() {
		Collections.shuffle(cards);
	}
	
	/**
	 * 最後にカードを追加する。
	 * 
	 * @param card
	 * 				追加するカード
	 */
	public void addCard(Card card) {
		cards.add(card);
	}
	
	/**
	 * 指定された場所にカードを追加する。
	 * 
	 * @param i
	 * 				挿入位置
	 * @param card
	 * 				追加するカード
	 */
	public void addCard(int i, Card card) {   // iは  Listのindex + 1  なので注意
		cards.add((i-1), card);
	}
	
	/**
	 * 一番上のカードを抜き取る、
	 * 
	 * @return カード
	 */
	public Card takeCard() {
		Card c = cards.get(0);
		cards.remove(0);
		return c;
	}
	
	/**
	 * 指定された位置のカードを抜き取る。
	 * 
	 * @param i
	 * 				位置
	 * @return カード
	 */
	public Card takeCard(int i) {
		Card c = cards.get(i-1);
		cards.remove(i-1);
		return c;
	}
	
	/**
	 * 指定された位置のカードを見る。
	 * 
	 * @param i
	 * 				位置
	 * @return カード
	 */
	public Card seeCard(int i) {
		return cards.get(i-1);
	}
	
	/**
	 * 指定された絵柄と数字に対応するカードが何番目にあるかを返す。なければ0を返す。
	 * 
	 * @param suit
	 * 				絵柄
	 * @param number
	 * 				数字
	 * @return 位置
	 */
	public int searchCard(int suit, int number) {
		Card c = new Card(suit, number);
		return cards.indexOf(c) + 1;
	}
	
	/**
	 * デッキが空かどうか判定する。
	 * 
	 * @return 空かどうか
	 */
	public boolean isEmpty() {
		return cards.isEmpty();
	}
	
	/**
	 * カードの枚数を数える。
	 * 
	 * @return カードの枚数
	 */
	public int size() {
		return cards.size();
	}
	
	/**
	 * デッキ内のすべてのカードを画面に出力する。
	 */
	public void showAllCards() {
		for(Card c: cards) {
			System.out.println(c.toString());
		}
	}
	
	/**
	 * デッキ内のすべてのカードをリストとして返す。
	 * 
	 * @return カードのリスト
	 */
	public List<Card> getAllCards() {
		List<Card> l = cards;
		return l;
	}
		

}
