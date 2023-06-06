package se.t2094572.card.entity;

import java.util.Random;

/**
 * Playerクラスを継承するCPUクラス。カードを選ぶメソッドを持つ。
 * 
 * @author Yuto Kizawa
 * @version 0.1, 2022-07-22
 * @since JDK1.8
 * 
 */
public class CPU extends Player{
	private Random r = new Random();

	/**
	 * 名前を指定してCPUインスタンスを生成する。
	 * 
	 * @param name
	 * 		名前
	 */
	public CPU(String name) {
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
		selection = r.nextInt(size) + 1;
		return selection;
	}
}
