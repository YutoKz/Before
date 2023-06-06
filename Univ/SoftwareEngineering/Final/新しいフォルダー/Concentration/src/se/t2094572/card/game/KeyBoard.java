package se.t2094572.card.game;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

/**
 * キーボードの入力を支援するユーティリティクラス
 * @author Masahide Nakamura
 * @version 0.1, 2018-05-20
 * @since JDK1.8
 */
public class KeyBoard {

	/**
	 * 整数を入力する．例外が出れば再入力をうながす．
	 * @return 入力された整数
	 */
	public static int inputNumber() {
		int num;

		try {
			String line = inputString();
			num = Integer.parseInt(line);
		} catch (NumberFormatException e) {
			System.err.println("フォーマット例外．整数を入力してください．");
			num = inputNumber();
		}

		return num;
	}

	/**
	 * 文字列を入力する．例外が出れば再入力をうながす．
	 * @return 入力された文字列
	 */
	public static String inputString() {
		String str = "";

		BufferedReader br;
		try {
			br = new BufferedReader(new InputStreamReader(System.in, "UTF-8"));
			str = br.readLine();
		} catch (UnsupportedEncodingException e1) {
			System.err.println("サポートされない文字コード．再入力：");
			str = inputString();

		} catch (IOException e) {
			System.err.println("入出力例外です．再入力：");
			str = inputString();
		}
		return str;

	}



}

