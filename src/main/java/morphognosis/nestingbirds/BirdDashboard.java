// For conditions of distribution and use, see copyright notice in Morphognosis.java

// The nesting bird dashboard.

package morphognosis.nestingbirds;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import morphognosis.nestingbirds.Bird.ORIENTATION;

public class BirdDashboard extends JFrame
{
   private static final long serialVersionUID = 0L;

   // Components.
   StatusPanel statusPanel;
   FoodPanel   foodPanel;

   // Target bird.
   Bird bird;

   // Nesting birds display.
   NestingBirdsDisplay nestingbirdsDisplay;

   // Constructor.
   public BirdDashboard(Bird bird, NestingBirdsDisplay nestingbirdsDisplay)
   {
      this.bird = bird;
      this.nestingbirdsDisplay = nestingbirdsDisplay;

      if (bird.gender == Bird.MALE)
      {
         setTitle("Male bird");
      }
      else
      {
         setTitle("Female bird");
      }
      addWindowListener(new WindowAdapter()
                        {
                           public void windowClosing(WindowEvent e) { close(); }
                        }
                        );
      JPanel basePanel = (JPanel)getContentPane();
      basePanel.setLayout(new BoxLayout(basePanel, BoxLayout.Y_AXIS));
      statusPanel = new StatusPanel();
      basePanel.add(statusPanel);
      foodPanel = new FoodPanel();
      basePanel.add(foodPanel);
      pack();
      setLocation();
      setVisible(true);
      update();
   }


   void setLocation()
   {
      Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
      int       w   = getSize().width;
      int       h   = getSize().height;
      int       x   = (int)((float)(dim.width - w) * 0.9f);
      int       y   = (dim.height - h) / 2;

      setLocation(x, y);
   }


   // Update dashboard.
   void update()
   {
      // Update status.
      statusPanel.update();
   }


   // Open the dashboard.
   void open()
   {
      setVisible(true);
   }


   // Close the dashboard.
   void close()
   {
      setVisible(false);
   }


   // Status panel.
   class StatusPanel extends JPanel
   {
      private static final long serialVersionUID = 0L;

      // Components.
      JTextField currentLocaleText;
      JTextField currentObjectText;
      JTextField leftLocaleText;
      JTextField leftObjectText;
      JTextField forwardLocaleText;
      JTextField forwardObjectText;
      JTextField rightLocaleText;
      JTextField rightObjectText;
      JTextField matePresentText;
      JTextField wantFoodText;
      JTextField wantStoneText;
      JTextField orientationText;
      JTextField foodText;
      JTextField hasObjectText;
      JTextField responseText;

      // Constructor.
      public StatusPanel()
      {
         setLayout(new BorderLayout());
         setBorder(BorderFactory.createTitledBorder(
                      BorderFactory.createLineBorder(Color.black),
                      "State"));
         JPanel sensorsPanel = new JPanel();
         sensorsPanel.setLayout(new BoxLayout(sensorsPanel, BoxLayout.Y_AXIS));
         sensorsPanel.setBorder(BorderFactory.createTitledBorder(
                                   BorderFactory.createLineBorder(Color.black),
                                   "Sensors"));
         add(sensorsPanel, BorderLayout.NORTH);
         JPanel currentCellPanel = new JPanel();
         currentCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         currentCellPanel.setBorder(BorderFactory.createTitledBorder(
                                       BorderFactory.createLineBorder(Color.black),
                                       "Current cell"));
         sensorsPanel.add(currentCellPanel);
         currentCellPanel.add(new JLabel("Locale:"));
         currentLocaleText = new JTextField(10);
         currentLocaleText.setEditable(false);
         currentCellPanel.add(currentLocaleText);
         currentCellPanel.add(new JLabel("Object:"));
         currentObjectText = new JTextField(10);
         currentObjectText.setEditable(false);
         currentCellPanel.add(currentObjectText);
         JPanel leftCellPanel = new JPanel();
         leftCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         leftCellPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Left cell"));
         sensorsPanel.add(leftCellPanel);
         leftCellPanel.add(new JLabel("Locale:"));
         leftLocaleText = new JTextField(10);
         leftLocaleText.setEditable(false);
         leftCellPanel.add(leftLocaleText);
         leftCellPanel.add(new JLabel("Object:"));
         leftObjectText = new JTextField(10);
         leftObjectText.setEditable(false);
         leftCellPanel.add(leftObjectText);
         JPanel forwardCellPanel = new JPanel();
         forwardCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         forwardCellPanel.setBorder(BorderFactory.createTitledBorder(
                                       BorderFactory.createLineBorder(Color.black),
                                       "Forward cell"));
         sensorsPanel.add(forwardCellPanel);
         forwardCellPanel.add(new JLabel("Locale:"));
         forwardLocaleText = new JTextField(10);
         forwardLocaleText.setEditable(false);
         forwardCellPanel.add(forwardLocaleText);
         forwardCellPanel.add(new JLabel("Object:"));
         forwardObjectText = new JTextField(10);
         forwardObjectText.setEditable(false);
         forwardCellPanel.add(forwardObjectText);
         JPanel rightCellPanel = new JPanel();
         rightCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         rightCellPanel.setBorder(BorderFactory.createTitledBorder(
                                     BorderFactory.createLineBorder(Color.black),
                                     "Right cell"));
         sensorsPanel.add(rightCellPanel);
         rightCellPanel.add(new JLabel("Locale:"));
         rightLocaleText = new JTextField(10);
         rightLocaleText.setEditable(false);
         rightCellPanel.add(rightLocaleText);
         rightCellPanel.add(new JLabel("Object:"));
         rightObjectText = new JTextField(10);
         rightObjectText.setEditable(false);
         rightCellPanel.add(rightObjectText);
         JPanel mateSensorPanel = new JPanel();
         mateSensorPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(mateSensorPanel);
         mateSensorPanel.add(new JLabel("Mate present:"));
         matePresentText = new JTextField(10);
         matePresentText.setEditable(false);
         mateSensorPanel.add(matePresentText);
         if (bird.gender == Bird.MALE)
         {
            mateSensorPanel.add(new JLabel("Want food:"));
            wantFoodText = new JTextField(5);
            wantFoodText.setEditable(false);
            mateSensorPanel.add(wantFoodText);
            mateSensorPanel.add(new JLabel("Want stone:"));
            wantStoneText = new JTextField(5);
            wantStoneText.setEditable(false);
            mateSensorPanel.add(wantStoneText);
         }
         JPanel responsePanel = new JPanel();
         responsePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         responsePanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Response"));
         add(responsePanel, BorderLayout.CENTER);
         responsePanel.add(new JLabel("Response:"));
         responseText = new JTextField(25);
         responseText.setEditable(false);
         responsePanel.add(responseText);
         JPanel statePanel = new JPanel();
         statePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         statePanel.setBorder(BorderFactory.createTitledBorder(
                                 BorderFactory.createLineBorder(Color.black),
                                 "Internal"));
         add(statePanel, BorderLayout.SOUTH);
         statePanel.add(new JLabel("Orientation: "));
         orientationText = new JTextField(10);
         orientationText.setEditable(false);
         statePanel.add(orientationText);
         statePanel.add(new JLabel("Food: "));
         foodText = new JTextField(10);
         foodText.setEditable(false);
         statePanel.add(foodText);
         statePanel.add(new JLabel("Has object: "));
         hasObjectText = new JTextField(10);
         hasObjectText.setEditable(false);
         statePanel.add(hasObjectText);
      }


      // Update.
      public void update()
      {
         // Get sensor values.
         switch (bird.sensors[Bird.CURRENT_LOCALE_SENSOR])
         {
         case NestingBirds.LOCALE.DESERT:
            currentLocaleText.setText("desert");
            break;

         case NestingBirds.LOCALE.FOREST:
            currentLocaleText.setText("forest");
            break;

         case NestingBirds.LOCALE.PLAIN:
            currentLocaleText.setText("plain");
            break;
         }
         switch (bird.sensors[Bird.CURRENT_OBJECT_SENSOR])
         {
         case NestingBirds.OBJECT.NO_OBJECT:
            currentObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            currentObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            currentObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            currentObjectText.setText("egg");
            break;
         }
         switch (bird.sensors[Bird.LEFT_LOCALE_SENSOR])
         {
         case NestingBirds.LOCALE.DESERT:
            leftLocaleText.setText("desert");
            break;

         case NestingBirds.LOCALE.FOREST:
            leftLocaleText.setText("forest");
            break;

         case NestingBirds.LOCALE.PLAIN:
            leftLocaleText.setText("plain");
            break;
         }
         switch (bird.sensors[Bird.LEFT_OBJECT_SENSOR])
         {
         case NestingBirds.OBJECT.NO_OBJECT:
            leftObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            leftObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            leftObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            leftObjectText.setText("egg");
            break;
         }
         switch (bird.sensors[Bird.FORWARD_LOCALE_SENSOR])
         {
         case NestingBirds.LOCALE.DESERT:
            forwardLocaleText.setText("desert");
            break;

         case NestingBirds.LOCALE.FOREST:
            forwardLocaleText.setText("forest");
            break;

         case NestingBirds.LOCALE.PLAIN:
            forwardLocaleText.setText("plain");
            break;
         }
         switch (bird.sensors[Bird.FORWARD_OBJECT_SENSOR])
         {
         case NestingBirds.OBJECT.NO_OBJECT:
            forwardObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            forwardObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            forwardObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            forwardObjectText.setText("egg");
            break;
         }
         switch (bird.sensors[Bird.RIGHT_LOCALE_SENSOR])
         {
         case NestingBirds.LOCALE.DESERT:
            rightLocaleText.setText("desert");
            break;

         case NestingBirds.LOCALE.FOREST:
            rightLocaleText.setText("forest");
            break;

         case NestingBirds.LOCALE.PLAIN:
            rightLocaleText.setText("plain");
            break;
         }
         switch (bird.sensors[Bird.RIGHT_OBJECT_SENSOR])
         {
         case NestingBirds.OBJECT.NO_OBJECT:
            rightObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            rightObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            rightObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            rightObjectText.setText("egg");
            break;
         }
         switch (bird.sensors[Bird.MATE_PROXIMITY_SENSOR])
         {
         case Bird.MATE_PROXIMITY_UNKNOWN:
            matePresentText.setText("unknown");
            break;

         case Bird.MATE_PROXIMITY_PRESENT:
            matePresentText.setText("present");
            break;

         case Bird.MATE_PROXIMITY_LEFT:
            matePresentText.setText("left");
            break;

         case Bird.MATE_PROXIMITY_FORWARD:
            matePresentText.setText("forward");
            break;

         case Bird.MATE_PROXIMITY_RIGHT:
            matePresentText.setText("right");
            break;
         }
         if (bird.gender == Bird.MALE)
         {
            if (bird.sensors[MaleBird.WANT_FOOD_SENSOR] == 0)
            {
               wantFoodText.setText("false");
            }
            else
            {
               wantFoodText.setText("true");
            }
            if (bird.sensors[MaleBird.WANT_STONE_SENSOR] == 0)
            {
               wantStoneText.setText("false");
            }
            else
            {
               wantStoneText.setText("true");
            }
         }

         // Internal state.
         switch (bird.orientation)
         {
         case ORIENTATION.NORTH:
            orientationText.setText("NORTH");
            break;

         case ORIENTATION.SOUTH:
            orientationText.setText("SOUTH");
            break;

         case ORIENTATION.EAST:
            orientationText.setText("EAST");
            break;

         case ORIENTATION.WEST:
            orientationText.setText("WEST");
            break;
         }
         foodText.setText(bird.food + "");
         switch (bird.hasObject)
         {
         case NestingBirds.OBJECT.NO_OBJECT:
            hasObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            hasObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            hasObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            hasObjectText.setText("egg");
            break;
         }

         // Response.
         if (bird.gender == Bird.MALE)
         {
            responseText.setText(MaleBird.RESPONSE.toString(bird.response));
         }
         else
         {
            responseText.setText(FemaleBird.RESPONSE.toString(bird.response));
         }
      }
   }

   // Food panel.
   class FoodPanel extends JPanel implements ActionListener, ItemListener
   {
      private static final long serialVersionUID = 0L;

      JTextField foodText;
      JButton    foodButton;
      JTextField foodDurationText;
      JButton    foodDurationButton;
      JCheckBox  randomizeFoodLevelCheckBox;

      // Constructor.
      public FoodPanel()
      {
         setBorder(BorderFactory.createTitledBorder(
                      BorderFactory.createLineBorder(Color.black),
                      "Food"));
         setLayout(new FlowLayout(FlowLayout.LEFT));
         JLabel initialFoodLabel = new JLabel("Food: ");
         add(initialFoodLabel);
         foodText = new JTextField(5);
         if (bird.gender == Bird.MALE)
         {
            foodText.setText(MaleBird.INITIAL_FOOD + "");
         }
         else
         {
            foodText.setText(FemaleBird.INITIAL_FOOD + "");
         }
         add(foodText);
         foodButton = new JButton("Set");
         foodButton.addActionListener(this);
         add(foodButton);
         JLabel foodDurationLabel = new JLabel("Food duration: ");
         add(foodDurationLabel);
         foodDurationText = new JTextField(5);
         if (bird.gender == Bird.MALE)
         {
            foodDurationText.setText(MaleBird.FOOD_DURATION + "");
         }
         else
         {
            foodDurationText.setText(FemaleBird.FOOD_DURATION + "");
         }
         add(foodDurationText);
         foodDurationButton = new JButton("Set");
         foodDurationButton.addActionListener(this);
         add(foodDurationButton);
         randomizeFoodLevelCheckBox = new JCheckBox("Randomize food level", false);
         randomizeFoodLevelCheckBox.addItemListener(this);
         add(randomizeFoodLevelCheckBox);
      }


      @Override
      public void actionPerformed(ActionEvent e)
      {
         if (e.getSource() == foodButton)
         {
            String text = foodText.getText();

            if ((text != null) && !text.isEmpty())
            {
               try
               {
                  int amount = Integer.parseInt(text);
                  if (amount < 0)
                  {
                     JOptionPane.showMessageDialog(this, "Invalid food value");
                     return;
                  }
                  if (bird.gender == Bird.MALE)
                  {
                     if (amount > MaleBird.FOOD_DURATION)
                     {
                        JOptionPane.showMessageDialog(this, "Food cannot be greater than food duration");
                        return;
                     }
                     bird.food = amount;
                  }
                  else
                  {
                     if (amount > FemaleBird.FOOD_DURATION)
                     {
                        JOptionPane.showMessageDialog(this, "Food cannot be greater than food duration");
                        return;
                     }
                     bird.food = amount;
                  }
               }
               catch (Exception ex)
               {
                  JOptionPane.showMessageDialog(this, "Invalid food value");
               }
            }
            return;
         }

         if (e.getSource() == foodDurationButton)
         {
            String text = foodDurationText.getText();

            if ((text != null) && !text.isEmpty())
            {
               try
               {
                  int duration = Integer.parseInt(text);
                  if (duration < 0)
                  {
                     JOptionPane.showMessageDialog(this, "Invalid food duration value");
                     return;
                  }
                  if (bird.gender == Bird.MALE)
                  {
                     MaleBird.FOOD_DURATION = duration;
                     if (bird.food > duration)
                     {
                        bird.food = duration;
                        foodText.setText(duration + "");
                     }
                  }
                  else
                  {
                     FemaleBird.FOOD_DURATION = duration;
                     if (bird.food > duration)
                     {
                        bird.food = duration;
                        foodText.setText(duration + "");
                     }
                  }
               }
               catch (Exception ex)
               {
                  JOptionPane.showMessageDialog(this, "Invalid food duration value");
               }
            }
            return;
         }
      }


      @Override
      public void itemStateChanged(ItemEvent e)
      {
         if (e.getSource() == randomizeFoodLevelCheckBox)
         {
            if (bird.gender == Bird.MALE)
            {
               MaleBird.RANDOMIZE_FOOD_LEVEL = randomizeFoodLevelCheckBox.isSelected();
            }
            else
            {
               FemaleBird.RANDOMIZE_FOOD_LEVEL = randomizeFoodLevelCheckBox.isSelected();
            }
         }
      }
   }
}
