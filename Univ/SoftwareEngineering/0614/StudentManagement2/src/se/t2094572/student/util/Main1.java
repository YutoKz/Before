package se.t2094572.student.util;

import java.util.TreeMap;

import se.t2094572.student.entity.Student;

public class Main1 {

	public static void main(String[] args) {
		
		TreeMap<String, Student> map = StudentMapFactory.create("student_list2.csv");
		StudentMapFactory.store(map, "student_list2_copy.csv");
		System.out.println("Completed.");
	}

}
