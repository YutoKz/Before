import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class SE0419_01 {
	
	String callName = "ジョーチ";
	
	public void hello()
	{
		System.out.println("こんにちは！私は"+ callName +"と申します。");
	}
	
	public void askBD()
	{
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		int month = 0, day = 0;
		
		try {
			System.out.println("あなたの誕生日(月日)を教えてください！");
			
			System.out.print("月：");
			String line = br.readLine();
			month = Integer.parseInt(line);
			
			System.out.print("日：");
			line = br.readLine();
			day = Integer.parseInt(line);
			System.out.println("あなたは" + month + "月" + day + "日生まれですね！");
			
			int md = month * 100 + day;
			if((md>=101&&md<=119)||(md>=1222&&md<=1231))
			{
				System.out.println("あなたは山羊座ですね！");
			}
			else if(md>=120&&md<=218)
			{
				System.out.println("あなたは水瓶座ですね！");
			}
			else if(md>=219&&md<=320)
			{
				System.out.println("あなたは魚座ですね！");
			}
			else if(md>=321&&md<=419)
			{
				System.out.println("あなたは牡羊座ですね！");
			}
			else if(md>=420&&md<=520)
			{
				System.out.println("あなたは牡牛座ですね！");
			}
			else if(md>=521&&md<=621)
			{
				System.out.println("あなたは双子座ですね！");
			}
			else if(md>=622&&md<=722)
			{
				System.out.println("あなたはかに座ですね！");
			}
			else if(md>=723&&md<=822)
			{
				System.out.println("あなたはしし座ですね！");
			}
			else if(md>=823&&md<=922)
			{
				System.out.println("あなたは乙女座ですね！");
			}
			else if(md>=923&&md<=1023)
			{
				System.out.println("あなたはてんびん座ですね！");
			}
			else if(md>=1024&&md<=1122)
			{
				System.out.println("あなたはさそり座ですね！");
			}
			else if(md>=1123&&md<=1221)
			{
				System.out.println("あなたはいて座ですね！");
			}
		} catch (IOException e) {
			System.err.println("入出力例外 ： プログラム終了");
			System.exit(1);
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		SE0419_01 s = new SE0419_01();
		
		s.hello();
		s.askBD();
	}

}