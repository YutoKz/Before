package se.t2094572.card.game;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import se.t2094572.card.entity.CardDeck;
import se.t2094572.card.entity.Player;

public class ConcentrationGame {
	private List<Player> players;
	private CardDeck deck;
	private ArrayList<Integer> selection = new ArrayList<Integer>();
	
	public ConcentrationGame() {
		players = new ArrayList<Player>();
		deck = new CardDeck();
		deck.createFullDeck();
		deck.shuffle();
	}
	
	public void addPlayer(Player player) {
		players.add(player);
		System.out.printf("%sさんをゲームに登録しました。\n", player.getName());
	}
	
	public void shufflePlayers() {
		System.out.println("プレイヤーの順番は次のように決まりました。");		
		Collections.shuffle(players);
		for (Player p: players) {
			System.out.println(p.getName());;
		}
		System.out.println();
	}
	
	public void startGame() {   // 未完成　最後に
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
	
	public void play(Player p) {
		if(deck.size() == 0) {
			return;
		}
		int c1, c2;
		selection = p.selectCards(deck.size());
		c1 = selection.get(0);
		c2 = selection.get(1);
		if(deck.seeCard(c1).getNumber() == deck.seeCard(c2).getNumber()) {
			p.addScore();
			deck.takeCard(c1);
			deck.takeCard(c2);
			play(p);
		}
		return;
	}
	
	public void showAllScores() {
		System.out.print("\n最終結果\n");
		for (Player p: players) {
			p.showScore();
		}
	}
	
	public List<Player> getPlayers() {
		return players;
	}
	
}
