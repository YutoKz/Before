package se.t2094572.student.util;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.TreeMap;

import se.t2094572.student.entity.InternationalStudent;
import se.t2094572.student.entity.RegularStudent;
import se.t2094572.student.entity.RobotStudent;
import se.t2094572.student.entity.Student;
import se.t2094572.student.entity.WorkingStudent;


/**
 * 学籍番号をキー、学生情報
 * 
 * @author Yuto Kizawa
 * @version 0.1, 2022-06-17
 * @since JDK1.8
 * 
 */
public class StudentMapFactory {
	
	/**
	 * ファイルから学籍番号と学生情報を読み込み、それを格納したTreeMapを返す。
	 * 
	 * @param fileName
	 * 			読み込むファイルの名前
	 * 
	 * @return 読み込んだ学生の情報を格納したTreeMap
	 */
	public static TreeMap<String, Student> create(String fileName)
	{
		TreeMap<String, Student> map = new TreeMap<String, Student>();

		try {
			BufferedReader br = new BufferedReader(new FileReader(fileName));
			String line;

			while ((line = br.readLine()) != null) {
				String[] data = line.split(",");
			
				switch(data[3]) {
				case "0":
					Student s = new RegularStudent(data[0], data[1], Integer.parseInt(data[2]));
					map.put(data[0], s);
					break;
				case "1":
					// System.out.println((Integer.parseInt(data[5]) == 1)? "1":"0");
					
					InternationalStudent is = new InternationalStudent(data[0], data[1], Integer.parseInt(data[2]), data[4], (Integer.parseInt(data[5]) == 1)? true:false);
					map.put(data[0], is);
					break;
				case "2":
					WorkingStudent ws = new WorkingStudent(data[0], data[1], Integer.parseInt(data[2]), data[4]);
					map.put(data[0], ws);
					break;
				case "3":
					RobotStudent rs = new RobotStudent(data[0], data[1], Integer.parseInt(data[2]), data[4], data[5]);
					map.put(data[0], rs);
					break;
				}
			}
			br.close();
			
		} catch (FileNotFoundException e) {
			System.err.println("error : ファイルが見つかりません　終了");
			System.exit(1);
		} catch (IOException e) {
			System.err.println("error : 入出力例外　終了");
			System.exit(1);
		}

		return map;
	}
	
	
	
	/**
	 * 受け取ったマップを指定されたファイル名のcsvファイルに書き出す。
	 * 
	 * @param map
	 * 			学籍番号をキー、学生情報を値とするマップ
	 * 
	 * @param fileName
	 * 			書き込むファイル名
	 */
	public static void store(TreeMap<String, Student> map, String fileName)
	{
		try
		{
			PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(fileName)));
			
			for(String id: map.keySet())
			{
				Student s = map.get(id);
				pw.printf("%s,%s,%d,", s.getId(), s.getName(), s.getCredit());
				
				if(s instanceof RegularStudent)
				{
					pw.printf("0,,");
				}
				else if(s instanceof InternationalStudent)
				{
					InternationalStudent is = (InternationalStudent)s;
					pw.printf("1,%s,%s", is.getCountry(), (is.getKokuhi() == true)? "1":"0");
				}
				else if(s instanceof WorkingStudent)
				{
					WorkingStudent ws = (WorkingStudent)s;
					pw.printf("2,%s,", ws.getCompany());
				}
				else if(s instanceof RobotStudent)
				{
					RobotStudent ros = (RobotStudent)s;
					pw.printf("3,%s,%s", ros.getVendor(), ros.getAlgorithm());
				}
				
				pw.println("");
			}
			
			pw.close();
		}
		catch (FileNotFoundException e)
		{
			System.err.println("ファイルが見つかりませんでした。プログラムを終了");
			System.exit(1);
		}
		catch (IOException e)
		{
			System.err.println("入出力例外につき、プログラムを終了");
		}
	}

}
