// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds display.

package morphognosis.nestingbirds;

import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.MediaTracker;
import java.awt.Point;
import java.awt.ScrollPane;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Objects;
import java.util.concurrent.TimeUnit;

import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

import morphognosis.nestingbirds.NestingBirds.RESPONSE_DRIVER;

public class NestingBirdsDisplay extends JFrame implements Runnable, ActionListener, MouseListener
{
   private static final long serialVersionUID = 0L;

   // Usage.
   public static final String Usage =
      "Usage:\n" +
      "    java morphognosis.nestingbirds.NestingBirdsDisplay\n" +
      "      [-steps <steps> (default=single step)]\n" +
      "      [-responseDriver <autopilot | bird> (default=autopilot)]\n" +
      "      [-maleInitialFood <amount> (default=" + MaleBird.INITIAL_FOOD + ")]\n" +
      "      [-femaleInitialFood <amount> (default=" + FemaleBird.INITIAL_FOOD + ")]\n" +
      "      [-maleFoodDuration <amount> (default=" + MaleBird.FOOD_DURATION + ")]\n" +
      "      [-femaleFoodDuration <amount> (default=" + FemaleBird.FOOD_DURATION + ")]\n" +
      "      [-randomizeMaleFoodLevel (food level probabilistically increases 0-" + MaleBird.FOOD_DURATION + " upon eating food)]\n" +
      "      [-randomizeFemaleFoodLevel (food level probabilistically increases 0-" + FemaleBird.FOOD_DURATION + " upon eating food)]\n" +
      "      [-writeMaleDataset (write dataset file: " + NestingBirds.MALE_DATASET_FILE_NAME + ".csv)]\n" +
      "      [-writeFemaleDataset (write dataset file: " + NestingBirds.FEMALE_DATASET_FILE_NAME + ".csv)]\n" +
      "      [-verbose <true | false> (default=false)]\n" +
      "      [-randomSeed <seed> (default=" + NestingBirds.RANDOM_NUMBER_SEED + ")]\n" +
      "      [-version]";

   // Nesting birds.
   public NestingBirds nestingbirds;

   // Milliseconds between display updates.
   static final int DISPLAY_UPDATE_DELAY = 50;

   // Dimensions.
   static final Dimension SCREEN_SIZE        = new Dimension(652, 680);
   static final Dimension CANVAS_SIZE        = new Dimension(630, 630);
   static final Dimension CONTROL_PANEL_SIZE = new Dimension(652, 100);
   static final Dimension CELL_SIZE          = new Dimension(30, 30);
   static final Dimension LOCALE_SIZE        = new Dimension(15, 15);
   static final Dimension OBJECT_SIZE        = new Dimension(20, 20);
   static final Dimension SMALL_BIRD_SIZE    = new Dimension(15, 15);
   static final Dimension BIRD_SIZE          = new Dimension(80, 80);

   // Screen size.
   Dimension screenSize;

   // Display.
   Graphics   graphics;
   ScrollPane canvasScroll;
   Dimension  canvasScrollSize;
   Canvas     canvas;
   Graphics   canvasGraphics;
   Image      worldImage;
   Graphics   worldImageGraphics;
   Dimension  worldImageSize;
   Point      femaleStatusLocation;
   Point      femaleImageLocation;
   Point      femaleObjectLocation;
   Point      maleStatusLocation;
   Point      maleImageLocation;
   Point      maleObjectLocation;
   Point      statusInfoOffset;
   Point      localeImageOffset;
   Point      objectImageOffset;
   Point      femaleImageOffset;
   Point      maleImageOffset;
   Thread     displayThread;
   String     message = "Click bird for dashboard";

   // JComboBox item.
   public class Item
   {
      public int     id;
      private String description;

      public Item(int id, String description)
      {
         this.id          = id;
         this.description = description;
      }


      @Override
      public String toString()
      {
         return(description);
      }
   };

   // Control panel.
   JPanel          controlPanel;
   Dimension       controlPanelSize;
   JComboBox<Item> responseDriverChoice;
   JComboBox<Item> femaleResponseChoice;
   JComboBox<Item> maleResponseChoice;
   JButton         stepButton;
   JLabel          stepCounterLabel;
   int             stepCounter;

   // Images
   Image maleImage;
   Image maleSmallImage;
   Image femaleImage;
   Image femaleSmallImage;
   Image treeImage;
   Image cactusImage;
   Image mouseImage;
   Image stoneImage;
   Image eggImage;
   Image maleNetImage;
   Image femaleNetImage;

   // Font.
   Font        font = new Font(Font.MONOSPACED, Font.PLAIN, 12);
   FontMetrics fontMetrics;
   int         fontAscent;
   int         fontWidth;
   int         fontHeight;

   // Dashboards.
   BirdDashboard maleDashboard;
   BirdDashboard femaleDashboard;

   // Steps.
   static int steps = 0;

   // Constructor.
   public NestingBirdsDisplay(NestingBirds nestingbirds)
   {
      this.nestingbirds = nestingbirds;

      // Set up display.
      setTitle("Nesting birds");
      setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      screenSize = SCREEN_SIZE;
      setSize(screenSize);
      setLayout(new BorderLayout());

      // Create control panel.
      controlPanel     = new JPanel();
      controlPanelSize = CONTROL_PANEL_SIZE;
      controlPanel.setBounds(0, 0, controlPanelSize.width, controlPanelSize.height);
      add(controlPanel, BorderLayout.NORTH);
      controlPanel.add(newLabel("Response: driver ="));
      responseDriverChoice = newComboBox();
      responseDriverChoice.setOpaque(true);
      responseDriverChoice.addItem(new Item(NestingBirds.RESPONSE_DRIVER.AUTOPILOT,
                                            NestingBirds.RESPONSE_DRIVER.toString(NestingBirds.RESPONSE_DRIVER.AUTOPILOT)));
      responseDriverChoice.addItem(new Item(NestingBirds.RESPONSE_DRIVER.BIRD,
                                            NestingBirds.RESPONSE_DRIVER.toString(NestingBirds.RESPONSE_DRIVER.BIRD)));
      responseDriverChoice.addItem(new Item(NestingBirds.RESPONSE_DRIVER.MANUAL,
                                            NestingBirds.RESPONSE_DRIVER.toString(NestingBirds.RESPONSE_DRIVER.MANUAL)));
      controlPanel.add(responseDriverChoice);
      controlPanel.add(newLabel("female ="));
      femaleResponseChoice = newComboBox();
      femaleResponseChoice.setOpaque(true);
      femaleResponseChoice.addItem(new Item(Bird.RESPONSE.DO_NOTHING, Bird.RESPONSE.toString(Bird.RESPONSE.DO_NOTHING)));
      femaleResponseChoice.addItem(new Item(Bird.RESPONSE.EAT, Bird.RESPONSE.toString(Bird.RESPONSE.EAT)));
      femaleResponseChoice.addItem(new Item(Bird.RESPONSE.GET, Bird.RESPONSE.toString(Bird.RESPONSE.GET)));
      femaleResponseChoice.addItem(new Item(Bird.RESPONSE.PUT, Bird.RESPONSE.toString(Bird.RESPONSE.PUT)));
      femaleResponseChoice.addItem(new Item(Bird.RESPONSE.TOSS, Bird.RESPONSE.toString(Bird.RESPONSE.TOSS)));
      femaleResponseChoice.addItem(new Item(Bird.RESPONSE.MOVE, Bird.RESPONSE.toString(Bird.RESPONSE.MOVE)));
      femaleResponseChoice.addItem(new Item(Bird.RESPONSE.TURN_RIGHT, Bird.RESPONSE.toString(Bird.RESPONSE.TURN_RIGHT)));
      femaleResponseChoice.addItem(new Item(Bird.RESPONSE.TURN_LEFT, Bird.RESPONSE.toString(Bird.RESPONSE.TURN_LEFT)));
      femaleResponseChoice.addItem(new Item(FemaleBird.RESPONSE.WANT_FOOD, FemaleBird.RESPONSE.toString(FemaleBird.RESPONSE.WANT_FOOD)));
      femaleResponseChoice.addItem(new Item(FemaleBird.RESPONSE.WANT_STONE, FemaleBird.RESPONSE.toString(FemaleBird.RESPONSE.WANT_STONE)));
      femaleResponseChoice.addItem(new Item(FemaleBird.RESPONSE.LAY_EGG, FemaleBird.RESPONSE.toString(FemaleBird.RESPONSE.LAY_EGG)));
      controlPanel.add(femaleResponseChoice);
      controlPanel.add(newLabel("male ="));
      maleResponseChoice = newComboBox();
      maleResponseChoice.setOpaque(true);
      maleResponseChoice.addItem(new Item(Bird.RESPONSE.DO_NOTHING, Bird.RESPONSE.toString(Bird.RESPONSE.DO_NOTHING)));
      maleResponseChoice.addItem(new Item(Bird.RESPONSE.EAT, Bird.RESPONSE.toString(Bird.RESPONSE.EAT)));
      maleResponseChoice.addItem(new Item(Bird.RESPONSE.GET, Bird.RESPONSE.toString(Bird.RESPONSE.GET)));
      maleResponseChoice.addItem(new Item(Bird.RESPONSE.PUT, Bird.RESPONSE.toString(Bird.RESPONSE.PUT)));
      maleResponseChoice.addItem(new Item(Bird.RESPONSE.TOSS, Bird.RESPONSE.toString(Bird.RESPONSE.TOSS)));
      maleResponseChoice.addItem(new Item(Bird.RESPONSE.MOVE, Bird.RESPONSE.toString(Bird.RESPONSE.MOVE)));
      maleResponseChoice.addItem(new Item(Bird.RESPONSE.TURN_RIGHT, Bird.RESPONSE.toString(Bird.RESPONSE.TURN_RIGHT)));
      maleResponseChoice.addItem(new Item(Bird.RESPONSE.TURN_LEFT, Bird.RESPONSE.toString(Bird.RESPONSE.TURN_LEFT)));
      maleResponseChoice.addItem(new Item(MaleBird.RESPONSE.GIVE_FOOD, MaleBird.RESPONSE.toString(MaleBird.RESPONSE.GIVE_FOOD)));
      maleResponseChoice.addItem(new Item(MaleBird.RESPONSE.GIVE_STONE, MaleBird.RESPONSE.toString(MaleBird.RESPONSE.GIVE_STONE)));
      controlPanel.add(maleResponseChoice);
      stepButton = newButton("Step");
      stepButton.addActionListener(this);
      controlPanel.add(stepButton);
      stepCounterLabel = newLabel("= 0");
      controlPanel.add(stepCounterLabel);
      stepCounter     = 0;
      maleDashboard   = null;
      femaleDashboard = null;

      // Create world display.
      canvasScroll     = new ScrollPane();
      canvasScrollSize = new Dimension(screenSize.width, (int)((double)screenSize.height * .95));
      canvasScroll.setBounds(0, 0, canvasScrollSize.width, canvasScrollSize.height);
      canvas = new Canvas();
      canvas.setBounds(0, controlPanelSize.height, CANVAS_SIZE.width, CANVAS_SIZE.height + controlPanelSize.height);
      canvas.addMouseListener(this);
      add(canvasScroll, BorderLayout.SOUTH);
      canvasScroll.add(canvas, null);

      // Get images.
      MediaTracker tracker = new MediaTracker(this);
      Image        image   = loadImage("male.gif");
      tracker.addImage(image, 0);
      maleImage = image.getScaledInstance(BIRD_SIZE.width,
                                          BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(maleImage, 0);
      maleSmallImage = image.getScaledInstance(SMALL_BIRD_SIZE.width,
                                               SMALL_BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(maleSmallImage, 0);
      image = loadImage("female.gif");
      tracker.addImage(image, 0);
      femaleImage = image.getScaledInstance(BIRD_SIZE.width,
                                            BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(femaleImage, 0);
      femaleSmallImage = image.getScaledInstance(SMALL_BIRD_SIZE.width,
                                                 SMALL_BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(femaleSmallImage, 0);
      image = loadImage("tree.gif");
      tracker.addImage(image, 0);
      treeImage = image.getScaledInstance(LOCALE_SIZE.width,
                                          LOCALE_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(treeImage, 0);
      image = loadImage("cactus.gif");
      tracker.addImage(image, 0);
      cactusImage = image.getScaledInstance(LOCALE_SIZE.width,
                                            LOCALE_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(cactusImage, 0);
      image = loadImage("mouse.gif");
      tracker.addImage(image, 0);
      mouseImage = image.getScaledInstance(OBJECT_SIZE.width,
                                           OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(mouseImage, 0);
      image = loadImage("stone.gif");
      tracker.addImage(image, 0);
      stoneImage = image.getScaledInstance(OBJECT_SIZE.width,
                                           OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(stoneImage, 0);
      image = loadImage("egg.gif");
      tracker.addImage(image, 0);
      eggImage = image.getScaledInstance(OBJECT_SIZE.width,
                                         OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(eggImage, 0);
      try
      {
         tracker.waitForAll();
      }
      catch (InterruptedException e)
      {
         JOptionPane.showMessageDialog(this, "Image loading interrupted: " + e.getMessage());
         System.exit(1);
      }

      // Make display visible.
      pack();
      setLocation();
      setVisible(true);
   }


   // Make label with font.
   private JLabel newLabel(String text)
   {
      JLabel label = new JLabel(text);

      label.setFont(font);
      return(label);
   }


   // Make button with font.
   private JButton newButton(String text)
   {
      JButton button = new JButton(text);

      button.setFont(font);
      return(button);
   }


   // Make combo box with font.
   private JComboBox<Item> newComboBox()
   {
      JComboBox<Item> box = new JComboBox<Item>();
      box.setFont(font);
      return(box);
   }


   public void setLocation()
   {
      Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
      int       w   = getSize().width;
      int       h   = getSize().height;
      int       x   = (dim.width - w) / 4;
      int       y   = (dim.height - h) / 2;

      setLocation(x, y);
   }


   // Start.
   public void start()
   {
      // Create display update thread.
      if (displayThread == null)
      {
         displayThread = new Thread(this);
         displayThread.start();
      }
   }


   // Stop.
   public void stop()
   {
      if (displayThread != null)
      {
         displayThread = null;
      }
   }


   // Run.
   public void run()
   {
      // Lower this thread's priority and get the current time.
      Thread.currentThread().setPriority(Thread.MIN_PRIORITY);

      // Display update loop.
      while (Thread.currentThread() == displayThread)
      {
         updateDisplay();

         try
         {
            Thread.sleep(DISPLAY_UPDATE_DELAY);
         }
         catch (InterruptedException e) { break; }
      }
   }


   // Update display.
   public void updateDisplay()
   {
      int    x, y, x2, y2;
      String s;

      // Initialize graphics.
      if (graphics == null)
      {
         if (!initGraphics()) { return; }
      }

      // Clear.
      worldImageGraphics.setColor(Color.white);
      worldImageGraphics.fillRect(0, 0, worldImageSize.width, worldImageSize.height);

      // Draw world.
      worldImageGraphics.setColor(Color.black);
      y2 = worldImageSize.height;
      int h = CONTROL_PANEL_SIZE.height;
      for (x = x2 = 0; x < NestingBirds.width; x++, x2 += CELL_SIZE.width)
      {
         worldImageGraphics.drawLine(x2, h, x2, y2);
      }
      worldImageGraphics.drawLine(x2, h, x2, y2);
      x2 = NestingBirds.width * CELL_SIZE.width;
      for (y = 0, y2 = h; y < NestingBirds.height; y++, y2 += CELL_SIZE.height)
      {
         worldImageGraphics.drawLine(0, y2, x2, y2);
      }
      worldImageGraphics.drawLine(0, y2 - 1, x2, y2 - 1);
      for (x = x2 = 0; x < NestingBirds.width; x++, x2 += CELL_SIZE.width)
      {
         for (y = 0, y2 = h; y < NestingBirds.height; y++, y2 += CELL_SIZE.height)
         {
            switch (nestingbirds.world[x][y].locale)
            {
            case NestingBirds.LOCALE.DESERT:
               worldImageGraphics.drawImage(cactusImage, x2 + localeImageOffset.x,
                                            y2 + localeImageOffset.y, this);
               break;

            case NestingBirds.LOCALE.FOREST:
               worldImageGraphics.drawImage(treeImage, x2 + localeImageOffset.x,
                                            y2 + localeImageOffset.y, this);
               break;

            case NestingBirds.LOCALE.PLAIN:
               break;
            }

            switch (nestingbirds.world[x][y].object)
            {
            case NestingBirds.OBJECT.NO_OBJECT:
               break;

            case NestingBirds.OBJECT.EGG:
               worldImageGraphics.drawImage(eggImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;

            case NestingBirds.OBJECT.MOUSE:
               worldImageGraphics.drawImage(mouseImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;

            case NestingBirds.OBJECT.STONE:
               worldImageGraphics.drawImage(stoneImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;
            }
         }
      }

      // Female status.
      worldImageGraphics.drawImage(femaleImage, femaleImageLocation.x,
                                   femaleImageLocation.y, this);
      s = "Female";
      x = femaleImageLocation.x + ((BIRD_SIZE.width - fontMetrics.stringWidth(s)) / 2);
      y = femaleImageLocation.y + BIRD_SIZE.height + fontAscent;
      worldImageGraphics.drawString(s, x, y);
      if (nestingbirds.female.hasObject == NestingBirds.OBJECT.MOUSE)
      {
         worldImageGraphics.drawImage(mouseImage, femaleObjectLocation.x,
                                      femaleObjectLocation.y, this);
      }
      else if (nestingbirds.female.hasObject == NestingBirds.OBJECT.STONE)
      {
         worldImageGraphics.drawImage(stoneImage, femaleObjectLocation.x,
                                      femaleObjectLocation.y, this);
      }
      s = "Orientation: " + Bird.ORIENTATION.toString(nestingbirds.female.orientation);
      x = femaleStatusLocation.x + statusInfoOffset.x;
      y = femaleStatusLocation.y + statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      s  = "Food: " + nestingbirds.female.food;
      y += fontAscent + 2;
      worldImageGraphics.drawString(s, x, y);
      x2 = nestingbirds.female.x * CELL_SIZE.width;
      y2 = (nestingbirds.female.y * CELL_SIZE.height) + h;
      worldImageGraphics.drawImage(femaleSmallImage, x2 + femaleImageOffset.x,
                                   y2 + femaleImageOffset.y, this);

      // Male status.
      worldImageGraphics.drawImage(maleImage, maleImageLocation.x,
                                   maleImageLocation.y, this);
      s = "Male";
      x = maleImageLocation.x + ((BIRD_SIZE.width - fontMetrics.stringWidth(s)) / 2);
      y = maleImageLocation.y + BIRD_SIZE.height + fontAscent;
      worldImageGraphics.drawString(s, x, y);
      if (nestingbirds.male.hasObject == NestingBirds.OBJECT.MOUSE)
      {
         worldImageGraphics.drawImage(mouseImage, maleObjectLocation.x,
                                      maleObjectLocation.y, this);
      }
      else if (nestingbirds.male.hasObject == NestingBirds.OBJECT.STONE)
      {
         worldImageGraphics.drawImage(stoneImage, maleObjectLocation.x,
                                      maleObjectLocation.y, this);
      }
      s = "Orientation: " + Bird.ORIENTATION.toString(nestingbirds.male.orientation);
      x = maleStatusLocation.x + statusInfoOffset.x;
      y = maleStatusLocation.y + statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      s  = "Food: " + nestingbirds.male.food;
      y += fontAscent + 2;
      worldImageGraphics.drawString(s, x, y);
      x2 = nestingbirds.male.x * CELL_SIZE.width;
      y2 = (nestingbirds.male.y * CELL_SIZE.height) + h;
      worldImageGraphics.drawImage(maleSmallImage, x2 + maleImageOffset.x,
                                   y2 + maleImageOffset.y, this);

      // Show message?
      if (message != null)
      {
         worldImageGraphics.setColor(Color.white);
         int w = fontMetrics.stringWidth(message);
         h = fontMetrics.getAscent() + fontMetrics.getDescent();
         worldImageGraphics.fillRect((worldImageSize.width - w) / 2, (worldImageSize.height / 6) - h, w, h);
         worldImageGraphics.setColor(Color.black);
         worldImageGraphics.drawString(message,
                                       (worldImageSize.width - w) / 2, worldImageSize.height / 6);
      }

      // Copy to display.
      canvasGraphics.drawImage(worldImage, 0, 0, this);
   }


   // Initialize graphics.
   boolean initGraphics()
   {
      int x, y;

      if (graphics != null) { return(true); }
      graphics = getGraphics();
      if (graphics == null) { return(false); }

      // Initialize font.
      graphics.setFont(font);
      fontMetrics = graphics.getFontMetrics();
      fontAscent  = fontMetrics.getMaxAscent();
      fontWidth   = fontMetrics.getMaxAdvance();
      fontHeight  = fontMetrics.getHeight();

      // Initialize graphics.
      if ((x = NestingBirds.width * CELL_SIZE.width) < CONTROL_PANEL_SIZE.width)
      {
         x = CONTROL_PANEL_SIZE.width;
      }
      x++;
      y = (NestingBirds.height * CELL_SIZE.height) + CONTROL_PANEL_SIZE.height;
      worldImageSize     = new Dimension(x, y);
      worldImage         = createImage(x, y);
      worldImageGraphics = worldImage.getGraphics();
      worldImageGraphics.setFont(font);
      y = 1;
      femaleStatusLocation = new Point(0, y);
      x = (int)((CONTROL_PANEL_SIZE.width - (2.5 * BIRD_SIZE.width)) / 2);
      femaleImageLocation = new Point(x, y);
      x = femaleImageLocation.x + ((BIRD_SIZE.width * 7) / 8);
      y = femaleImageLocation.y + (BIRD_SIZE.height / 8);
      femaleObjectLocation = new Point(x, y);
      x = 1;
      y = CELL_SIZE.height - SMALL_BIRD_SIZE.height;
      femaleImageOffset = new Point(x, y);
      x = (int)(femaleImageLocation.x + (1.5 * BIRD_SIZE.width));
      y = 1;
      maleImageLocation = new Point(x, y);
      x = maleImageLocation.x + BIRD_SIZE.width;
      maleStatusLocation = new Point(x, y);
      x = maleImageLocation.x - (BIRD_SIZE.width / 8) - 1;
      y = maleImageLocation.y + (BIRD_SIZE.height / 8);
      maleObjectLocation = new Point(x, y);
      x = CELL_SIZE.width - SMALL_BIRD_SIZE.width;
      y = CELL_SIZE.height - SMALL_BIRD_SIZE.height;
      maleImageOffset = new Point(x, y);
      x = 2;
      y = 2 + fontAscent;
      statusInfoOffset  = new Point(x, y);
      localeImageOffset = new Point(1, 1);
      x = (CELL_SIZE.width - OBJECT_SIZE.width) / 2;
      y = (CELL_SIZE.height - OBJECT_SIZE.height) / 2;
      objectImageOffset = new Point(x, y);
      canvasGraphics    = canvas.getGraphics();
      canvasGraphics.setFont(font);
      return(true);
   }


   // Action listener.
   @Override
   public void actionPerformed(ActionEvent e)
   {
      // Step button.
      if (e.getSource() == stepButton)
      {
         steps = 0;
         step();
      }
   }


   // Step.
   public synchronized void step()
   {
      stepCounter++;
      stepCounterLabel.setText("= " + stepCounter + "");
      if (NestingBirds.Verbose)
      {
         System.out.println("Step=" + stepCounter);
      }
      Item item = (Item)femaleResponseChoice.getSelectedItem();
      nestingbirds.female.response = item.id;
      item = (Item)maleResponseChoice.getSelectedItem();
      nestingbirds.male.response = item.id;
      item = (Item)responseDriverChoice.getSelectedItem();
      nestingbirds.setResponseDriver(item.id);
      nestingbirds.step();
      message = null;
      for (int i = 0; i < femaleResponseChoice.getItemCount(); i++)
      {
         item = femaleResponseChoice.getItemAt(i);
         if (item.id == nestingbirds.female.response)
         {
            femaleResponseChoice.setSelectedIndex(i);
            break;
         }
      }
      for (int i = 0; i < maleResponseChoice.getItemCount(); i++)
      {
         item = maleResponseChoice.getItemAt(i);
         if (item.id == nestingbirds.male.response)
         {
            maleResponseChoice.setSelectedIndex(i);
            break;
         }
      }
      if (femaleDashboard != null)
      {
         femaleDashboard.update();
      }
      if (maleDashboard != null)
      {
         maleDashboard.update();
      }
   }


   // Canvas mouse listener.
   @Override
   public void mouseClicked(MouseEvent e)
   {
      int x = e.getX();
      int y = e.getY();

      if ((x >= femaleImageLocation.x) && (x <= femaleImageLocation.x + BIRD_SIZE.width))
      {
         if ((y >= femaleImageLocation.y) && (y <= femaleImageLocation.y + BIRD_SIZE.height))
         {
            if (femaleDashboard == null)
            {
               femaleDashboard = new BirdDashboard(nestingbirds.female);
            }
            else
            {
               femaleDashboard.open();
            }
         }
      }
      if ((x >= maleImageLocation.x) && (x <= maleImageLocation.x + BIRD_SIZE.width))
      {
         if ((y >= maleImageLocation.y) && (y <= maleImageLocation.y + BIRD_SIZE.height))
         {
            if (maleDashboard == null)
            {
               maleDashboard = new BirdDashboard(nestingbirds.male);
            }
            else
            {
               maleDashboard.open();
            }
         }
      }
   }


   @Override
   public void mousePressed(MouseEvent e) {}


   @Override
   public void mouseReleased(MouseEvent e) {}


   @Override
   public void mouseEntered(MouseEvent e) {}


   @Override
   public void mouseExited(MouseEvent e) {}

   // Load image.
   Image loadImage(String name)
   {
      BufferedImage image    = null;
      String        protocol = this.getClass().getResource("").getProtocol();

      if (Objects.equals(protocol, "jar"))
      {
         try
         {
            image = ImageIO.read(getClass().getResource(name));
         }
         catch (Exception e)
         {
            JOptionPane.showMessageDialog(this, "Cannot load image for " + name + ": " + e.getMessage());
            System.exit(1);
         }
      }
      else
      {
         try
         {
            image = ImageIO.read(new File("res/images/" + name));
         }
         catch (Exception e)
         {
            JOptionPane.showMessageDialog(this, "Cannot load image for " + name + ": " + e.getMessage());
            System.exit(1);
         }
      }
      return(image);
   }


   // Main.
   public static void main(String[] args)
   {
      NestingBirds.Verbose = false;
      for (int i = 0; i < args.length; i++)
      {
         if (args[i].equals("-steps"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid steps option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               steps = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid steps option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (steps < 0)
            {
               System.err.println("Invalid steps option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-responseDriver"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid driver option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (args[i].equals("autopilot"))
            {
               NestingBirds.ResponseDriver = RESPONSE_DRIVER.AUTOPILOT;
            }
            else if (args[i].equals("bird"))
            {
               NestingBirds.ResponseDriver = RESPONSE_DRIVER.BIRD;
            }
            else
            {
               System.err.println("Invalid responseDriver option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-maleInitialFood"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid maleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               MaleBird.INITIAL_FOOD = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid maleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (MaleBird.INITIAL_FOOD < 0)
            {
               System.err.println("Invalid maleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-femaleInitialFood"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid femaleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               FemaleBird.INITIAL_FOOD = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid femaleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (FemaleBird.INITIAL_FOOD < 0)
            {
               System.err.println("Invalid femaleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-maleFoodDuration"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid maleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               MaleBird.FOOD_DURATION = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid maleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (MaleBird.FOOD_DURATION < 0)
            {
               System.err.println("Invalid maleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-femaleFoodDuration"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid femaleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               FemaleBird.FOOD_DURATION = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid femaleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (FemaleBird.FOOD_DURATION < 0)
            {
               System.err.println("Invalid femaleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-randomizeMaleFoodLevel"))
         {
            MaleBird.RANDOMIZE_FOOD_LEVEL = true;
            continue;
         }
         if (args[i].equals("-randomizeFemaleFoodLevel"))
         {
            FemaleBird.RANDOMIZE_FOOD_LEVEL = true;
            continue;
         }
         if (args[i].equals("-randomSeed"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid randomSeed option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               NestingBirds.RANDOM_NUMBER_SEED = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid randomSeed option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (NestingBirds.RANDOM_NUMBER_SEED <= 0)
            {
               System.err.println("Invalid randomSeed option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-writeMaleDataset"))
         {
            if (((i + 1) < args.length) && !args[i + 1].startsWith("-"))
            {
               i++;
               if ((args[i] != null) && !args[i].isEmpty())
               {
                  NestingBirds.MALE_DATASET_FILE_NAME = args[i];
               }
               else
               {
                  System.err.println("Invalid writeMaleDataset file name");
                  System.err.println(Usage);
                  System.exit(1);
               }
            }
            try
            {
               NestingBirds.MaleDatasetWriter = new PrintWriter(new FileOutputStream(new File(NestingBirds.MALE_DATASET_FILE_NAME + ".csv")));
            }
            catch (IOException e)
            {
               System.err.println("Cannot open writeMaleDataset file: " + e.getMessage());
               System.err.println(Usage);
               System.exit(1);
            }
            Bird.writeDatasetHeader(NestingBirds.MaleDatasetWriter, Bird.MALE);
            continue;
         }
         if (args[i].equals("-writeFemaleDataset"))
         {
            if (((i + 1) < args.length) && !args[i + 1].startsWith("-"))
            {
               i++;
               if ((args[i] != null) && !args[i].isEmpty())
               {
                  NestingBirds.FEMALE_DATASET_FILE_NAME = args[i];
               }
               else
               {
                  System.err.println("Invalid writeFemaleDataset file name");
                  System.err.println(Usage);
                  System.exit(1);
               }
            }
            try
            {
               NestingBirds.FemaleDatasetWriter = new PrintWriter(new FileOutputStream(new File(NestingBirds.FEMALE_DATASET_FILE_NAME + ".csv")));
            }
            catch (IOException e)
            {
               System.err.println("Cannot open writeFemaleDataset file: " + e.getMessage());
               System.err.println(Usage);
               System.exit(1);
            }
            Bird.writeDatasetHeader(NestingBirds.FemaleDatasetWriter, Bird.FEMALE);
            continue;
         }
         if (args[i].equals("-verbose"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid verbose option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (args[i].equals("true"))
            {
               NestingBirds.Verbose = true;
            }
            else if (args[i].equals("false"))
            {
               NestingBirds.Verbose = false;
            }
            else
            {
               System.err.println("Invalid verbose option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-version"))
         {
            System.out.println("Nesting birds version = " + NestingBirds.VERSION);
            System.exit(0);
         }
         if (args[i].equals("-help") || args[i].equals("-h") || args[i].equals("-?"))
         {
            System.out.println(Usage);
            System.exit(0);
         }
         System.err.println(Usage);
         System.exit(1);
      }
      if (MaleBird.INITIAL_FOOD > MaleBird.FOOD_DURATION)
      {
         System.err.println("Male initial food cannot be greater than food duration");
         System.err.println(Usage);
         System.exit(1);
      }
      if (FemaleBird.INITIAL_FOOD > FemaleBird.FOOD_DURATION)
      {
         System.err.println("Female initial food cannot be greater than food duration");
         System.err.println(Usage);
         System.exit(1);
      }

      // Create nesting birds.
      NestingBirds nestingbirds = new NestingBirds();

      // Create display.
      NestingBirdsDisplay nestingbirdsDisplay = new NestingBirdsDisplay(nestingbirds);
      nestingbirdsDisplay.responseDriverChoice.setSelectedIndex(NestingBirds.ResponseDriver);

      // Start.
      nestingbirdsDisplay.start();

      // Automatic steps?
      if (steps > 0)
      {
         try
         {
            TimeUnit.SECONDS.sleep(3);
         }
         catch (InterruptedException e) {
         }
         for (int i = 0; i < steps; i++)
         {
            nestingbirdsDisplay.step();
            try
            {
               TimeUnit.MILLISECONDS.sleep(100);
            }
            catch (InterruptedException e) {
            }
         }
      }
   }
}
