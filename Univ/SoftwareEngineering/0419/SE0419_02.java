import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;

public class SE0419_02{

	public static void main(String[] args) {
		// TODO 自動生成されたメソッド・スタブ
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		try {
			System.out.print("単語をコンマ区切りで入力：");
			String line = br.readLine();
			String [] data = line.split(","); 
			Arrays.sort(data);
			
			System.out.println("ソートします");
			for(String i: data)
				System.out.println(i);
			
		} catch (IOException e) {
			System.err.println("入出力例外：プログラムを終了します");
			System.exit(1);
		}
	}
}
