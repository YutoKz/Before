package se.t2094572.card.entity;

/**
 * 「Playerたるもの」を定義する抽象クラス。名前と手持ちの数を持つ。ユーザとCPUはこのクラスを継承する。
 * 
 * @author Yuto Kizawa
 * @version 0.1, 2022-07-22
 * @since JDK1.8
 * 
 */
public abstract class Player {
	/** 名前 */
	protected String name;
	/** 手持ちの数 */
	protected int score;
	
	/** 名前を指定して、Playerインスタンスを生成する。
	 *
	 * @param name
	 * 		名前 
	 */
	public Player(String name) {
		if (name.length()==0 || name==null) {
			this.name = "名無し";
		} else {
			this.name = name;
		}
		score = 0;
	}
	
	/**
	 * カードを選ぶ抽象メソッド。
	 *
	 * @param size
	 * 		現在のデッキの枚数
	 * 
	 * @return 上から何枚目か
	 */
	public abstract int selectCard(int size);
	
	/**
	 * 手持ちの数を表示する。
	 */
	public void showScore() {
		System.out.printf("%sさんの手持ちの数: %d枚\n", name, score);
	}
	
	/**
	 * 獲得した2枚を手持ちに加える。
	 */
	public void addScore() {
		score += 2;
 	}
	
	/**
	 * 名前を取得する。
	 * 
	 * @return 名前
	 */
	public String getName() {
		return name;
	}
	
	/**
	 * 手持ちの数を取得する。
	 * 
	 * @return 手持ちの数
	 */
	public int getScore() {
		return score;
	}
}

