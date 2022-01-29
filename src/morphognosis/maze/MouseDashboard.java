// For conditions of distribution and use, see copyright notice in Main.java

// Mouse dashboard.

package morphognosis.maze;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.TextArea;
import java.awt.Toolkit;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import morphognosis.MorphognosticDisplay;

public class MouseDashboard extends JFrame
{
   private static final long serialVersionUID = 0L;

   // Components.
   public StatePanel           statePanel;
   public MorphognosticDisplay morphognosticPanel;

   // Mouse.
   public Mouse mouse;

   // Quit.
   public boolean quit;

   // Constructor.
   public MouseDashboard(Mouse mouse)
   {
      this.mouse = mouse;

      setTitle("Mouse");
      quit = false;
      addWindowListener(new WindowAdapter()
                        {
                           public void windowClosing(WindowEvent e)
                           {
                              quit = true;
                           }
                        }
                        );
      JPanel basePanel = (JPanel)getContentPane();
      basePanel.setLayout(new BoxLayout(basePanel, BoxLayout.Y_AXIS));
      statePanel = new StatePanel();
      basePanel.add(statePanel);
      morphognosticPanel = new MorphognosticDisplay(mouse.morphognostics[mouse.morphognostics.length - 1]);
      basePanel.add(morphognosticPanel);
      pack();
      setLocation();
      setVisible(true);
      update();
   }


   // Set dashboard location.
   public void setLocation()
   {
      Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
      int       w   = getSize().width;
      int       h   = getSize().height;
      int       x   = (int)((float)(dim.width - w) * 0.9f);
      int       y   = (dim.height - h) / 2;

      setLocation(x, y);
   }


   // Update dashboard.
   public boolean update()
   {
      if (!quit)
      {
         statePanel.update();
         return(true);
      }
      else
      {
         return(false);
      }
   }


   // Status panel.
   public class StatePanel extends JPanel
   {
      private static final long serialVersionUID = 0L;

      // Components.
      public TextArea   sensorsText;
      public JTextField responseText;
      public JTextField responseDriverText;

      // Constructor.
      public StatePanel()
      {
         setLayout(new BorderLayout());
         setBorder(BorderFactory.createTitledBorder(
                      BorderFactory.createLineBorder(Color.black),
                      "State"));
         JPanel panel = new JPanel();
         panel.setLayout(new FlowLayout(FlowLayout.LEFT));
         add(panel, BorderLayout.NORTH);
         panel.add(new JLabel("Sensors:"));
         sensorsText = new TextArea("", 2, 40, TextArea.SCROLLBARS_HORIZONTAL_ONLY);
         sensorsText.setEditable(false);
         panel.add(sensorsText);
         JPanel responsePanel = new JPanel();
         responsePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         add(responsePanel, BorderLayout.CENTER);
         responsePanel.add(new JLabel("Response:"));
         responseText = new JTextField(15);
         responseText.setEditable(false);
         responsePanel.add(responseText);
         JPanel responseDriverPanel = new JPanel();
         responseDriverPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         add(responseDriverPanel, BorderLayout.SOUTH);
         responseDriverPanel.add(new JLabel("Response driver:"));
         responseDriverText = new JTextField(15);
         responseDriverText.setEditable(false);
         responseDriverPanel.add(responseDriverText);
      }


      // Update.
      public void update()
      {
         // Update sensors.
         float[] sensors = mouse.sensors;
         String sensorsString = "";
         for (int i = 0; i < sensors.length; i++)
         {
            if (sensors[i] == 0.0f)
            {
               sensorsString += "0 ";
            }
            else
            {
               sensorsString += "1 ";
            }
         }
         sensorsText.setText(sensorsString);

         // Update response.
         responseText.setText(Mouse.getResponseName(mouse.response));

         // Update response driver.
         switch (mouse.responseDriver)
         {
         case ResponseDriver.TRAINING_OVERRIDE:
            responseDriverText.setText("trainingOverride");
            break;

         case ResponseDriver.METAMORPH_DB:
            responseDriverText.setText("metamorphDB");
            break;

         case ResponseDriver.METAMORPH_NN:
            responseDriverText.setText("metamorphNN");
            break;
         }
      }
   }
}
