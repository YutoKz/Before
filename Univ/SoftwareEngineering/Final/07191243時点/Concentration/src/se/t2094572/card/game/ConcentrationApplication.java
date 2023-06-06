package se.t2094572.card.game;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import se.t2094572.card.entity.CPU;
import se.t2094572.card.entity.User;

public class ConcentrationApplication {
	private User user;	
	private static final String logFile = "jyanken.log";
	private ConcentrationGame game;
	
	public ConcentrationApplication() {
		
	}
	
	public void startApplication() {
		initialize();
		doGame();
		storeScores();
	}

	private void initialize(){
		System.out.println("神経衰弱アプリを起動します。");
		System.out.print("名前を入力してください：　");
		String name = KeyBoard.inputString();

		int n;
		do {
			System.out.print("何人で遊びますか(2以上の整数)：");
			n = KeyBoard.inputNumber();
		} while (n < 2);
		game = new ConcentrationGame();

		user = new User(name);
		game.addPlayer(user);

		for (int i = 1; i < n; i++) {
			game.addPlayer(new CPU("CPU" + i));
		}
		
		game.shufflePlayers();
		
	}

	private void doGame() {
		game.startGame();
	}

	private void storeScores() {
		try {
			PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(logFile, true)));
			Calendar cl = Calendar.getInstance();

			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			String date = sdf.format(cl.getTime());
			int s = user.getScore();
			
			String out = String.format("%s,%s,%d,%d", date, user.getName(), game.getPlayers().size(), s);

			System.out.printf("%sさんの対戦成績をファイルに追記します。\n", user.getName());
			System.out.println(out);
			
			pw.println(out);
			pw.close();
			System.out.println("完了しました。");
		} catch (IOException e) {
			System.out.println(logFile + "：ファイル書き込み中、IO例外です。");
			e.printStackTrace();
		}
		
	}
	
	

}
