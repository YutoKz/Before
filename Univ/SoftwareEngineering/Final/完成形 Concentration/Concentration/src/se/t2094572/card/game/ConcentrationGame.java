package se.t2094572.card.game;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import se.t2094572.card.entity.CardDeck;
import se.t2094572.card.entity.Player;

/**
 * 神経衰弱ゲームクラス。プレイヤーのリスト、カードデッキのインスタンスを持つ。
 * 
 * @author Yuto Kizawa
 * @version 0.1, 2022-07-22
 * @since JDK1.8
 * 
 */
public class ConcentrationGame {
	/** プレイヤーのリスト */
	private List<Player> players;
	/** カードデッキ */
	private CardDeck deck;
	private String className;
	
	/**
	 * 神経衰弱ゲームを作成する。ジョーカー以外のフルデッキを用意し、シャッフルしておく。
	 */
	public ConcentrationGame() {
		players = new ArrayList<Player>();
		deck = new CardDeck();
		deck.createFullDeck();
		deck.shuffle();
	}
	
	/**
	 * プレイヤーをゲームに追加する。
	 * 
	 * @param player
	 * 		追加するプレイヤー
	 */
	public void addPlayer(Player player) {
		players.add(player);
		System.out.printf("%sさんをゲームに登録しました。\n", player.getName());
	}
	
	/**
	 * プレイヤーのリストをシャッフルして、ランダムに順番を決める。
	 */
	public void shufflePlayers() {
		System.out.println("プレイヤーの順番は次のように決まりました。");		
		Collections.shuffle(players);
		for (Player p: players) {
			System.out.println(p.getName());;
		}
		System.out.println();
	}
	
	
	/**
	 * ゲームを開始する。場に出ているカードがなくなるまで繰り返し、最後に成績を表示する。
	 */
	public void startGame() {
		if (players.size() < 2) {
			System.out.println("プレイヤが足りません。ゲームを開始できません");
			return;
		}

		System.out.println("神経衰弱を開始します。");
		
		while(true) {
			for (Player p: players) {
				play(p);
				if(deck.size() == 0) {
					break;
				}
			}
			if(deck.size() == 0) {
				break;
			}
		}
		
		showAllScores();

	}
	
	/**
	 * プレイヤーが自分のターンにカードを2枚引く。数字が同じならばもう一度行う。
	 * 
	 * @param p
	 * 		プレイヤー
	 */
	public void play(Player p) {
		if(deck.size() == 0) {
			return;
		}
		int c1, c2;
		className = p.getClass().getName();
		
		if(className == "se.t2094572.card.entity.User") {
			System.out.println("--------------------");
			System.out.printf("あなた [残り%3d枚]\n", deck.size());
		}
		else {
			System.out.println(p.getName());
		}
		
		c1 = p.selectCard(deck.size());
		if(className == "se.t2094572.card.entity.User") {
			System.out.print("   　　　");
			System.out.println(deck.seeCard(c1).toString());
		}
		else {
			System.out.printf("%3d番目\t%s\n", c1, deck.seeCard(c1).toString());
		}
		
		do {
			c2 = p.selectCard(deck.size());
		} while(c2 == c1);
		if(className == "se.t2094572.card.entity.User") {
			System.out.print("   　　　");
			System.out.println(deck.seeCard(c2).toString());
			System.out.println("--------------------");
		}
		else {
			System.out.printf("%3d番目\t%s\n", c2, deck.seeCard(c2).toString());
		}
		
		if(deck.seeCard(c1).getNumber() == deck.seeCard(c2).getNumber()) {
			p.addScore();
			if(c1 > c2) {
				deck.takeCard(c1);
				deck.takeCard(c2);
			}
			else {
				deck.takeCard(c2);
				deck.takeCard(c1);
			}
			play(p);
		}
		return;
	}
	
	/**
	 * プレイヤー全員の手持ちの数を表示する。
	 */
	public void showAllScores() {
		System.out.print("\n最終結果\n");
		for (Player p: players) {
			p.showScore();
		}
	}
	
	/**
	 * 全プレイヤーのリストを取得する。
	 * 
	 * @return 全プレイヤーのリスト
	 */
	public List<Player> getPlayers() {
		return players;
	}
	
}
