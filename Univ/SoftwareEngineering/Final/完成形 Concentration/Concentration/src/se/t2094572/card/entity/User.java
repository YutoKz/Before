package se.t2094572.card.entity;

import se.t2094572.card.game.KeyBoard;

/**
 * Playerクラスを継承するユーザクラス。カードを選ぶメソッドを持つ。
 * 
 * @author Yuto Kizawa
 * @version 0.1, 2022-07-22
 * @since JDK1.8
 * 
 */
public class User extends Player {
	/**
	 * 名前を指定してユーザインタンスを生成する。 
	 *
	 * @param name
	 * 		名前
	 */
	public User(String name) {
		super(name);
	}
	/** 
	 * カードを選ぶ。
	 * 
	 * @param size
	 * 		現在のデッキの枚数
	 * 
	 * @return 上から何枚目か
 	 */
	@Override
	public int selectCard(int size) {
		int selection;
		System.out.print(" 何番目?:");
		
		do { 
			selection = KeyBoard.inputNumber();
		} while (selection < 1 || selection > size);
		return selection;
	}
}
