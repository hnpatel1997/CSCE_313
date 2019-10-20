import java.io.*;
import java.sql.*;
import java.util.ArrayList;
import java.util.Arrays;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;

import java.awt.*;
import java.awt.event.*;
public class Connect
{
	// JDBC driver name and database URL
	/*
   static final String JDBC_DRIVER = "org.postgresql.Driver";  
   static final String DB_URL = "jdbc:postgresql://db-315.cse.tamu.edu/hnpatel97_db";
   static final String USER = "hnpatel97";
   static final String PASS = "studentpwd";
	 */
	 
	static final String JDBC_DRIVER = "org.postgresql.Driver";  
	static final String DB_URL = "jdbc:postgresql://imdb.cfj1htplvy5o.us-east-1.rds.amazonaws.com/imdb";
	static final String USER = "alyssadng1";
	static final String PASS = "studentpwd";
	
	
	public static void main(String[] args) 
	{
		Connection conn = null;
		Statement stmt = null;
		String sql ="";
		String sql1 = "";
		try
		{
			//Open a connection
			Class.forName("org.postgresql.Driver");
			System.out.println("Connecting to database...");
			conn = DriverManager.getConnection(DB_URL,USER,PASS);
			stmt = conn.createStatement();
			//Creates the Window
			mainFrame mainWindow = new mainFrame();

			while(mainWindow.frameRun) {//main frame runs while frameRun is true or until close button is not pressed; 
				//frameRun turns false when close button is pressed in the main frame

				mainWindow.running = true; //reset

				while(mainWindow.running) {//loop ends when new sql is detected and running is set to false


					//System.out.println("Main frame is running");
					sql = mainFrame.passSQL();//when button is pressed new sql will be created and running will be set to false
					System.out.flush(); //synchronizes 
				}
				JOptionPane.showMessageDialog(null,sql); //shows sql command in message box
				//Get Data
				//stmt.executeUpdate(sql);
				sql1 = mainFrame.passYear();
				//Print out data
				ResultSet rs = null;
				if(sql1 == "")
					rs = stmt.executeQuery(sql); //rs stores results from database
				//STEP 7: Extract data from result set	 
				//Create JTable to store data
				JFrame f = new JFrame(); //new frame for table
				String col[] = mainFrame.passCol(); //column names of table
				JTable t = new JTable(new DefaultTableModel(null, col)); //creates table w/ initial column names
				t.setBounds(30,40,200,500);          
				JScrollPane sp=new JScrollPane(t);    
				f.add(sp);          
				f.setSize(600,400);    
				f.setVisible(true); 

				DefaultTableModel model = (DefaultTableModel) t.getModel();
				String cateTemp = mainFrame.passCate(); //category
				String movielist[] = new String[] {};
				int count = 0;
				if(sql1 == "") {
					while(rs.next()){
						//Retrieve by column name

						if((cateTemp == "Movies") || (cateTemp =="TV Shows")) {
							String movieID = rs.getString("movie_id");
							String ordering = rs.getString("ordering");
							String title = rs.getString("title");
							String region = rs.getString("region");
							String language = rs.getString("language");
							String types = rs.getString("types");
							int isoriginaltitle = rs.getInt("isoriginaltitle");
							//print 
							System.out.println(movieID + "\t" + ordering +
									"\t" + title + "\t" + region +
									"\t" + language + "\t" + types +
									"\t" + isoriginaltitle);
							//Display values
							Object[] colArray= new Object[] {movieID, ordering, title, region, language, types, isoriginaltitle};
							model.addRow(colArray); //adds row into the table
						}
						else if(cateTemp =="People") {
							String peopleID = rs.getString("people_id");
							String name = rs.getString("name");
							String birthyear = rs.getString("birthyear");
							String profession = rs.getString("profession");
							String knownTitles = rs.getString("known_titles");

							Object[] colArray = new Object[] {peopleID, name, birthyear, profession, knownTitles};
							model.addRow(colArray);
						}
						else if(cateTemp =="Ratings") {
							String movieId = rs.getString("movie_id");
							String averagerating = rs.getString("averagerating");
							String numvotes = rs.getString("numvotes");

							Object[] colArray = new Object[] {movieId, averagerating, numvotes};
							model.addRow(colArray);
						}
						else if(cateTemp == "Number of Episodes") {
							
							  String avg = rs.getString("count");
							  Object[] colArray = new Object[] {avg};
							  model.addRow(colArray);
						  }
						else if(cateTemp =="suggest") {

							//passCol = new String[] {"movie_id",  "titletype", "primarytitle", "originaltitle", "isadult", "startyear", "endyear", "runtimeminutes", "genres"};
							String title = rs.getString("primarytitle");
							//String runtimeminutes = rs.getString("runtimeminutes");
							//Object[] colArray = new Object[] {title, runtimeminutes};
							Object[] colArray = new Object[] {title};
							model.addRow(colArray);

						}
					}
				}
			
				if(cateTemp == "Degree of Seperation")
				{
					String person = "";
					String sql_1;
					String n1 = "Lauren Bacall";
					String n2 = "Ronald Harris"; // Dan Wallace // Ronald Harris
					String[] parse = sql1.split(",");
					n1 = parse[0];
					n2 = parse[1];
					String n1_true = n1;
					String n_temp = n1;
					System.out.println("Actor 1: " + n1 + "\tActor 2: " + n2 + "\n");
					String[] list_of_actors = null;
					int count1 = 0;
					String name_new1 = null;

					boolean x = true;

					while(x)
					{
						sql_1 = "SELECT * FROM people WHERE name = '" + n1 + "' LIMIT 1;";
						rs = stmt.executeQuery(sql_1);

						//STEP 7: Extract data from result set
						String movies = null;
						while(rs.next()){
							//Retrieve by column name
							String id  = rs.getString("name");
							movies = rs.getString("known_titles");
							person += "Name: " + id + "\tTitles: " + movies + "\n";
						}

						//System.out.println(person);

						String[] arrOfMovId = movies.split(",");

						for(String a: arrOfMovId)
						{
							//System.out.println(a);
						}

						String p1 = "";
						String p2 = "";
						String p3 = "";
						String p4 = "";

						String sql12 = "SELECT * FROM PEOPLE WHERE known_titles LIKE '%" + arrOfMovId[0] + "%';";
						ResultSet rs1 = stmt.executeQuery(sql12);

						String name = null;
						while(rs1.next())
						{
							name = rs1.getString("name");
							p1 += name + ",";
						}

						String sql11 = "SELECT * FROM PEOPLE WHERE known_titles LIKE '%" + arrOfMovId[1] + "%';";
						ResultSet rs11 = stmt.executeQuery(sql11);

						while(rs11.next())
						{
							name = rs11.getString("name");
							p2 += name + ",";
						}

						if(arrOfMovId.length > 2)
						{
							String sql_12 = "SELECT * FROM PEOPLE WHERE known_titles LIKE '%" + arrOfMovId[2] + "%';";
							ResultSet rs12 = stmt.executeQuery(sql_12);

							while(rs12.next())
							{
								name = rs12.getString("name");
								p3 += name + ",";
							}
						}

						if(arrOfMovId.length > 3)
						{
							String sql13 = "SELECT * FROM PEOPLE WHERE known_titles LIKE '%" + arrOfMovId[3] + "%';";
							ResultSet rs13 = stmt.executeQuery(sql13);

							while(rs13.next())
							{
								name = rs13.getString("name");
								p4 += name + ",";
							}
						}


						String output = null;
						String sql2 = null;
						if(p1.contains(n2))
						{
							//System.out.println("1\n");
							sql2 = "SELECT * FROM movies WHERE movie_id = '" + arrOfMovId[0] + "' AND (region = 'GB' OR region = 'US');";
							ResultSet rs2 = stmt.executeQuery(sql2);
							String name_new = null;
							while(rs2.next())
							{
								name_new = rs2.getString("title");
							}

							

							output = n1 + "\t" + n2 + "\t" + name_new; 
							System.out.println(output);
							System.out.println("\n");
							x = false;
						}
						else if(p2.contains(n2))
						{
							//System.out.println("2\n");
							sql2 = "SELECT * FROM movies WHERE movie_id = '" + arrOfMovId[1] + "' AND (region = 'GB' OR region = 'US');";
							ResultSet rs2 = stmt.executeQuery(sql2);
							String name_new = null;
							while(rs2.next())
							{
								name_new = rs2.getString("title");
							}
							output = n1 + "\t" + n2 + "\t" + name_new; 
							System.out.println(output);
							System.out.println("\n");
							x = false;
						}
						else if(p3.contains(n2))
						{
							//System.out.println("3\n");
							sql2 = "SELECT * FROM movies WHERE movie_id = '" + arrOfMovId[2] + "' AND (region = 'GB' OR region = 'US');";
							ResultSet rs2 = stmt.executeQuery(sql2);
							String name_new = null;
							while(rs2.next())
							{
								name_new = rs2.getString("title");
							}
							output = n1 + "\t" + n2 + "\t" + name_new; 
							System.out.println(output);
							System.out.println("\n");
							x = false;
						}
						else if(p4.contains(n2))
						{
							//System.out.println("4\n");
							sql2 = "SELECT * FROM movies WHERE movie_id = '" + arrOfMovId[3] + "' AND (region = 'GB' OR region = 'US');";
							ResultSet rs2 = stmt.executeQuery(sql2);
							String name_new = null;
							while(rs2.next())
							{
								name_new = rs2.getString("title");
							}
							output = n1 + "\t" + n2 + "\t" + name_new; 
							System.out.println(output);
							System.out.println("\n");
							x = false;
						}
						else
						{

							// Degree of seperation is greater than one
							// Loop this back to the top and search again
							String new_name = "";
							if(list_of_actors == null)
							{
								sql_1 = "SELECT * FROM people WHERE (known_titles LIKE '%tt0038355%' AND (profession LIKE '%actor%' OR profession LIKE '%actress%'));";
								rs = stmt.executeQuery(sql_1);
								String list_actors = null;
								while(rs.next()){
									new_name = rs.getString("name");
									list_actors += new_name + ",";
								}
								list_of_actors = list_actors.split(",");
							}
							n_temp = list_of_actors[count1];
							if (n_temp.contains(n1) | n_temp.contains(n1_true))
								count1++;
							n_temp = list_of_actors[count1];

							//System.out.println("5\n");
							sql2 = "SELECT * FROM movies WHERE movie_id = 'tt0038355' AND (region = 'GB' OR region = 'US');";
							ResultSet rs2 = stmt.executeQuery(sql2);

							while(rs2.next())
							{
								name_new1 = rs2.getString("title");
							}
							//output = n1_true + "\t" + n_temp + "\t" + name_new1; 
							//System.out.println(output);
							n1 = n_temp;
						}
					}
				}
				else if(cateTemp == "Search Year")
				{
					
					String list_of_actors = "";
					String[] array_of_actors = null;
					String x = sql1;
					String[] x1 = x.split("-");
					int r1 = Integer.parseInt(x1[0]);
					int r2 = Integer.parseInt(x1[1]);
					for(int b = r1; b <= r2; b++)
					{
						String c = Integer.toString(b);
						String sql11 = "SELECT * FROM title_details WHERE startyear = '" + c + "' AND titletype = 'movie';";
						rs = stmt.executeQuery(sql11);
						String movie_list = "";

						while(rs.next())
						{
							String movie = rs.getString("movie_id");
							movie_list += movie + ",";
						}
						String[] array_of_movie = movie_list.split(",");


						for(String a : array_of_movie)
						{

							//System.out.println(a);
							sql11 = "SELECT * FROM people WHERE known_titles LIKE '%" + a + "%' AND (profession LIKE '%actor%' OR profession LIKE '%actress%' OR profession LIKE '%director%');";
							ResultSet rs1 = stmt.executeQuery(sql11);
							while(rs1.next())
							{
								String actor = rs1.getString("name");
								if(!list_of_actors.contains(actor))
									list_of_actors += actor + ",";
							}
						}
					}

					array_of_actors = list_of_actors.split(",");
					JOptionPane.showMessageDialog(null,Arrays.toString(array_of_actors));
					System.out.println(Arrays.toString(array_of_actors));
				}
				

			}
			//STEP 8: Clean-up environment
			conn.close();
		}
		catch(SQLException se)
		{
			System.out.println("SQL Exception se");
			//Handle errors for JDBC
			se.printStackTrace();
		}catch(Exception e){
			System.out.println("Exception e");
			//Handle errors for Class.forName
			e.printStackTrace();
		}
		finally //finally block used to close resources
		{
			try
			{
				if(stmt!=null)
					stmt.close();
			}
			catch(SQLException se2)
			{
				// nothing we can do
			}
			try
			{
				if(conn!=null)
				{
					conn.close();
				}
			}catch(SQLException se)
			{
				se.printStackTrace();
			}
		}//end try
		System.out.println(" Goodbye!");
	}//end main
}//end FirstExample

class mainFrame implements ActionListener
{ 
	JFrame frame; 
	JPanel panel;
	JTextField findTextField;
	//to pass values to Connect class
	private static String sqlText="";
	private static String sqlYear="";
	private static String cate="";
	private static String[] passCol= {};
	//used for while loop
	public boolean running = true;
	public boolean frameRun = true;

	public mainFrame() 
	{   
		// creating instance of JFrame with name "Main Window" 

		frame=new JFrame("Main Window"); 

		//creates instance of search bar
		JTextField searchBar = new JTextField();
		searchBar.setBounds(300, 50, 200, 20); 
		searchBar.addActionListener(this);
		System.out.println(searchBar.getText());
		frame.add(searchBar);
		//creates instance of combo box
		String category[] = {"Movies", "TV Shows", "People","Ratings", "Number of Episodes", "Degree of Seperation", "Search Year"};
		JComboBox c1 = new JComboBox(category);
		c1.setBounds(150, 50, 150, 20);
		frame.add(c1);
		// creates instance of JButton 
		JButton button = new JButton("Search");
		button.setBounds(500, 50, 100, 20);

		button.addActionListener(new ActionListener() {
			/**
			 *
			 */
			/**
			 *
			 */
			@Override
			public void actionPerformed(ActionEvent e) 
			{
				cate = (String) c1.getItemAt(c1.getSelectedIndex()); //category
				String s = e.getActionCommand(); 
				String text = searchBar.getText(); //search bar text
				if(cate == "Movies") {
					sqlText = "SELECT * FROM movies WHERE title = "+"'" + text +"'"+" LIMIT 5;";
					passCol = new String[] {"movie_id", "ordering", "title", "region", "language", "types", "isoriginaltitle"};
					System.out.println(text);                	

				}
				else if(cate =="TV Shows") {
					sqlText = "SELECT * FROM movies WHERE title = "+"'" + text +"'"+" LIMIT 5;"; 
					passCol = new String[] {"movie_id", "ordering", "title", "region", "language", "types", "isoriginaltitle"};
					System.out.println(text);

				}
				else if(cate == "People") {
					sqlText = "SELECT * FROM people WHERE name = "+"'" + text + "'" + " LIMIT 5;";
					passCol = new String[] {"people_id", "name", "birthyear", "profession", "known_titles"};
					System.out.println(text);

				}
				else if(cate == "Ratings") {
					Double textD = Double.parseDouble(text);
					sqlText = "SELECT * FROM ratings WHERE averagerating = "+"'" + textD + "'" + " LIMIT 5;";
					passCol = new String[] {"movie_id", "averagerating", "numvotes"};
					System.out.println(textD);

				}
				else if(cate == "Number of Episodes") {
            		//sqlText = "SELECT birthyear FROM People WHERE name = "+"'" + text +"';";
            		sqlText = " SELECT COUNT(episodenumber) FROM episode "
            				+ "INNER JOIN title_details ON title_details.movie_id "
            				+ "= episode.movie_id AND title_details.primarytitle = "
            				+ "'" + text + "';";
            		
            			//	+ "UNION ALL SELECT COUNT(episodenumber) FROM test WHERE primarytitle = '"+ text +"';";
            		//setSeries(text);
            		passCol = new String[] {"count"};
            		System.out.println(text);
            		}
				else if(cate == "Degree of Seperation")
				{
					sqlYear = text;
				}
				else if(cate == "Search Year")
				{
					sqlYear = text;
					
				}
				running = false; //to end while loop
			}
		});

		JLabel ilabel = new JLabel("Get movie suggestions: ");
		ilabel.setBounds(128, 70, 500, 100); 
		frame.add(ilabel);
		
		JLabel glabel = new JLabel("Genre: ");
		glabel.setBounds(228, 90, 90, 100); 
		frame.add(glabel);
		JTextField genreBar = new JTextField();
		genreBar.setBounds(300, 130, 200, 20); 
		genreBar.addActionListener(this);

		JLabel ylabel = new JLabel("Year: ");
		ylabel.setBounds(236, 110, 90, 100); 
		frame.add(ylabel);
		JTextField yearBar = new JTextField();
		yearBar.setBounds(300, 150, 200, 20); 
		yearBar.addActionListener(this);

		JLabel alabel = new JLabel("Actor: ");
		alabel.setBounds(230, 130, 90, 100); 
		frame.add(alabel);
		JTextField actorBar = new JTextField();
		actorBar.setBounds(300, 170, 200, 20); 
		actorBar.addActionListener(this);
		
		JLabel olabel = new JLabel("***Optional*** ");
		olabel.setBounds(360, 150, 500, 100); 
		frame.add(olabel);
		
		JLabel tlabel = new JLabel("Minimum Movie Length: ");
		tlabel.setBounds(127, 170, 150, 100); 
		frame.add(tlabel);
		JTextField tBar = new JTextField();
		tBar.setBounds(300, 210, 200, 20); 
		tBar.addActionListener(this);

		JLabel ttlabel = new JLabel("Maximum Movie Length: ");
		ttlabel.setBounds(125, 190, 150, 100); 
		frame.add(ttlabel);
		JTextField ttBar = new JTextField();
		ttBar.setBounds(300, 230, 200, 20); 
		ttBar.addActionListener(this);
		
		frame.add(genreBar);
		frame.add(yearBar);
		frame.add(actorBar);
		frame.add(tBar);
		frame.add(ttBar);

		//creates instance of combo box
		// creates instance of JButton 
		JButton sbutton = new JButton("Suggest");
		sbutton.setBounds(500, 230, 100, 20);

		sbutton.addActionListener(new ActionListener() 
		{
			@Override
			public void actionPerformed(ActionEvent e) 
			{
				String s = e.getActionCommand(); 
				String movie = searchBar.getText(); //search bar text
				String genre = genreBar.getText();
				int year = Integer.parseInt(yearBar.getText());
				int min = 100;
				if(!tBar.getText().trim().isEmpty())
				min = Integer.parseInt(tBar.getText());
				int max = 300;
				if(!ttBar.getText().trim().isEmpty())
				max = Integer.parseInt(ttBar.getText());
				String actor = actorBar.getText();

				sqlText = "SELECT primarytitle FROM title_details "
						+ "WHERE startyear = " + year
						+ " AND genres = '" + genre + "'"
						+ " AND runtimeminutes >= " + min
						+ " AND runtimeminutes <= " + max
						+ " LIMIT 5;";
				//passCol = new String[] {"primarytitle", "runtimeminutes"};
				passCol = new String[] {"primarytitle"};
				//System.out.println(sqlText);
				cate = "suggest";
				running = false; //to end while loop
			}
		});


		JButton closeB = new JButton("Close");
		closeB.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) 
			{
				frameRun = false;
				//System.out.println("FrameRun is False");
				frame.dispose(); //close frame
				System.exit(0); //exit programs
			}
		});
		closeB.setBounds(500, 70, 100, 20);
		//Add button in frame
		frame.add(button);
		frame.add(sbutton);
		frame.add(closeB);

		//creates instance of text
		JLabel label = new JLabel("Search: ");
		label.setBounds(100, 10, 90, 100); 
		frame.add(label);

		// sets 800 width and 600 height 
		frame.setSize(800, 600); 

		// uses no layout managers 
		frame.setLayout(null); 

		// This is the closing condition
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// makes the frame visible
		frame.setVisible(true); 
	}
	public static String passSQL() //return sql to Connect class
	{
		return sqlText;
	}
	public static String passYear()
	{
		return sqlYear;
	}
	public static String passCate() { //pass categories
		return cate;
	}
	public static String[] passCol() { //pass colume names for jtable
		return passCol;
	}
	public void actionPerformed(ActionEvent e) 
	{ 
		//Nothing here yet
	} 
}
