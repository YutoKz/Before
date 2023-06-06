package se.t2094572.card.entity;

import java.util.ArrayList;

public abstract class Player {
	protected String name;
	protected int score;
	
	public Player(String name) {
		if (name.length()==0 || name==null) {
			this.name = "名無し";
		} else {
			this.name = name;
		}
		score = 0;
	}
	
	public abstract ArrayList<Integer> selectCards(int size);
		
	public void showScore() {
		System.out.printf("%sさんの手持ちの数: %d枚\n", name, score);
	}
	
	public void addScore() {
		score += 2;
 	}
	
	public String getName() {
		return name;
	}
	
	public int getScore() {
		return score;
	}
}

