import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

class GUI implements Runnable{
	private String title = "Window 1";
	private JFrame w;
	private JPanel pn;
	private int[] d = {360,430};
	private JButton[][] ttt_bns;
	private Game game;
	GUI(String val) {
		title = val;
		w = new JFrame(title);
		pn = new JPanel();
		pn.setLayout(null);
		w.add(pn);
		reset(); //create a new game
	}
	public void reset() {
		game = new Game(); //reset the game
		if (ttt_bns != null) updateBoard(game.state);
		else createBoard(game.state);
	}
	public void createBoard(String gamestate) { //initial board generation
		char[][] vals = T3Tree.build_board(gamestate);
		ttt_bns = new JButton[3][3];
		int tpos = 0;
		for (int y=0; y<3; y++) {
			for (int x=0; x<3; x++) {
				String lb = Character.toString(vals[y][x]).toUpperCase();
				ttt_bns[y][x] = new JButton(!lb.equals("N")?lb:"");
				ttt_bns[y][x].setBounds(20+(y*110),20+((2-x)*110),100,100);
				final int tpos_copy = tpos;
				ttt_bns[y][x].addActionListener(new ActionListener(){
					public void actionPerformed(ActionEvent e) {
						game.move(tpos_copy);
						updateBoard(game.state);
					}
				});
				pn.add(ttt_bns[y][x]);
				tpos++;
			}
		}
	}
	public void updateBoard(String gamestate) {
		char[][] vals = T3Tree.build_board(gamestate);
		for (int a=0; a<3; a++)
			for (int b=0; b<3; b++) {
				String lb = Character.toString(vals[a][b]).toUpperCase();
				ttt_bns[b][a].setText(!lb.equals("N")?lb:""); //update character
			}
	}
	public void run() { //initialization
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {}
		w.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		Dimension sn = Toolkit.getDefaultToolkit().getScreenSize();
		w.setBounds((sn.width-d[0])/2,(sn.height-d[1])/2,d[0],d[1]);
		w.setResizable(false);
		w.setVisible(true);
		JButton reset_b = new JButton("Reset");
		reset_b.setBounds(d[0]-100,d[1]-80,80,34);
		reset_b.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				reset();
			}
		});
		pn.add(reset_b);
		final JDialog dlg = new JDialog(w, "About", Dialog.ModalityType.TOOLKIT_MODAL);
		dlg.setBounds((sn.width-300)/2,(sn.height-150)/2,300,150);
		JPanel dlgtxt = new JPanel(){
			private static final long serialVersionUID = 1L;
			@Override
			protected void paintComponent(Graphics g) {
				super.paintComponent(g);
				Graphics2D g2d = (Graphics2D)g;
				g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
				String[] about_text = {"Tic-Tac-Toe","A simple game in Java.","2012 Princeton Ferro"};
				for (int i=0; i<about_text.length; i++) {
					int strlen = (int) g2d.getFontMetrics().getStringBounds(about_text[i], g2d).getWidth();
					g2d.drawString(about_text[i],(300-strlen)/2,30*(i+1));
				}
			}
		};
		dlg.add(dlgtxt);
		JButton about_b = new JButton("About");
		about_b.setBounds(d[0]-190,d[1]-80,80,34);
		about_b.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				dlg.setVisible(true);
			}
		});
		pn.add(about_b);
	}
}
