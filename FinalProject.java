//Kyaw Zin Thi Ha Final project
// This project asks the user for the year and it prints out the calendar year 
import java.util.Scanner;
import java.time.DayOfWeek;
import java.time.LocalDate;
import java.io.FileWriter;
import java.time.Month;
import java.io.IOException;
import java.util.Calendar;

public class FinalProject {
    
        private static final int[][] yearly = 
        {{1, 31}, {2, 28}, {3, 31}, {4, 30}, {5, 31}, {6, 30},
         {7, 31}, {8, 31}, {9, 30}, {10, 31}, {11, 30}, {12, 31}
        };
        public static void main(String[] args)
        {
            Scanner scanner = new Scanner(System.in);
            System.out.print("Enter the year: "); // Get year from the user
            int current = scanner.nextInt();
            System.out.println();
            displayCalendar(current);
            scanner.close();
            
        }
    
        private static final String[] monthNames =
        {
        "January", "February", "March", "April", "May", "June", 
        "July", "August", "September", "October", "November", "December"
        };

        public static boolean LeapYear(int year)
        {
            return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        }

        public static void displayCalendar(int year)
        {
            for(int i = 0; i < yearly.length; i++)
            {
                int monthNum = yearly[i][0];
                int lastDayOfMonth = yearly[i][1];
                Calendar calendar = Calendar.getInstance();
                if(monthNum == 2 && LeapYear(year))
                {
                    lastDayOfMonth = 29;
                }
                System.out.println(monthNames[i] + "  " + year);
                printDays(year, monthNum, lastDayOfMonth);
            }
        }
        
        public static void printDays(int year, int month, int lastDayMonth)
        {
            Calendar calendar = Calendar.getInstance();
            LocalDate firstDayMonth = LocalDate.of(year, month, 2);
            int start = firstDayMonth.getDayOfWeek().getValue();
            System.out.println(" Sun  Mon  Tue  Wed  Thu  Fri  Sat"); //Days of the months
            for(int i = 1;i < start; i++)
            {
                System.out.print("     ");
            }
            for(int day = 1; day <= lastDayMonth; day++)
            {
                System.out.printf("%4d ", day); //aligning the calendar format 
                if((start + day - 1) % 7 == 0)
                {
                    System.out.println();
                }
            }
            System.out.println("\n");
        }
     
    }