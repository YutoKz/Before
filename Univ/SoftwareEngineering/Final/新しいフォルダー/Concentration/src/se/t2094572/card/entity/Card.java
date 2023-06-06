package se.t2094572.card.entity;

/**
 * どのようなゲームでも使用可能な汎用的なトランプカードクラス。絵柄と数字を持つ。
 * 
 * @author Yuto Kizawa
 * @version 0.1, 2022-07-09
 * @since JDK1.8
 * 
 */
public class Card {
	/** 絵柄 */
	private int suit;
	/** 数字 */
	private int number;
	
	/** 
	 * 絵柄、数字を指定して、カードインスタンスを生成する。
	 * 
	 *  @param suit
	 *  			絵柄
	 *  @param number
	 *  			数字
 	 */
	public Card(int suit, int number) {
		this.suit = suit;
		this.number = number;
	}
	
	/**
	 * 絵柄を取得する。
	 * 
	 * @return 絵柄
	 */
	public int getSuit() {
		return suit;
	}
	
	/**
	 * 数字を取得する。
	 * 
	 * @return 数字
	 */
	public int getNumber() {
		return number;	
	}
	
	/**
	 * カード情報を整数表現に変換する。
	 * 
	 * @return カードの整数表現
	 */
	public int toIndex() {
		return getIndex(suit, number);
	}
	
	/**
	 * カード情報を文字列表現に変換する。
	 * 
	 * @return カードの文字列表現
	 */
	@Override
	public String toString() {
		return getString(suit, number);
	}
	
	/**
	 * カード情報を画面に出力する。
	 */
	public void show() {
		System.out.println(this.toString());
	}
	
	/**
	 * 絵柄と数字に対応する整数表現を返す。
	 * 
	 * @param suit
	 * 				絵柄
	 * @param number
	 * 				数字
	 * 
	 * @return カードの整数表現
	 */
	public static int getIndex(int suit, int number) {
		if(suit != -1) {
			return 13 * suit + number - 1;
		}
		else {
			return -1;
		}
	}
	
	/**
	 * 絵柄と数字に対応する文字列表現を返す。
	 * 
	 * @param suit
	 * 				絵柄
	 * @param number
	 * 				数字
	 * 
	 * @return カードの文字列表現
	 */
	public static String getString(int suit, int number) {
		String s;
		String n = "";
		
		switch(suit) {
		case -1:
			s = "ジョーカー"; break;
		case 0:
			s = "スペード"; break;
		case 1:
			s = "ダイヤ"; break;
		case 2:
			s = "ハート"; break;
		case 3:
			s = "クラブ"; break;
		default:
			s = "存在しないカード"; break;
		}
		
		if(number == 1) {
			n = "A";
		} 
		else if(number == 11) {
			n = "J";
		}
		else if(number == 12) {
			n = "Q";
		}
		else if(number == 13) {
			n = "K";
		}
		else if(number != 0){
			n = Integer.toString(number);
		}
		
		return String.format("%s%s", s, n);
	}


}

	
	
	

	

