#pragma once
#include <gtk/gtk.h>

using namespace std;

GtkBuilder *builder;

GtkWidget *window1;
GtkWidget *fixed1;
GtkWidget *button1, *button2, *button3;
GtkWidget *entry1, *entry2;
GtkWidget *label1, *label2, *label3, *label4, *label5;

GtkWidget *window2;
GtkWidget *fixed2;
GtkWidget *button4, *button5, *button6, *button7;
GtkWidget *entry3;
GtkWidget *label6, *label7, *label8, *label9, *label10, *label11;

GtkWidget *window3;
GtkWidget *fixed3;
GtkWidget *button8, *button9, *button10;
GtkWidget *label12, *label13, *label14, *label15, *label16, *label17, *label18, *label19, *label20;
GtkWidget *spin1, *spin2;
GtkWidget *check1, *check2, *check3, *check4, *check5, *check6, *check7;

GtkWidget *window4;
GtkWidget *fixed4;
GtkWidget *button11, *button12;
GtkWidget *label21, *label22, *label23, *label24, *label25, *label26;
GtkWidget *toggle1;

GtkWidget *window5;
GtkWidget *fixed5;
GtkWidget *button13, *button14;
GtkWidget *label27, *label28, *label29, *label30, *label31, *label32, *label33, *label34, *label37;
GtkWidget *radio1, *radio2, *radio3;

GtkWidget *window6;
GtkWidget *fixed6;
GtkWidget *button15;
GtkWidget *label35, *label36, *label38, *label39;

GtkWidget *window7;
GtkWidget *fixed7;
GtkWidget *button16;
GtkWidget *label40, *label41, *label42, *label43, *label44, *label45, *label46, *label47;

extern "C" G_MODULE_EXPORT void  on_button1_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void  on_button2_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void  on_button3_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void  on_button4_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void  on_button5_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void  on_button6_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void  on_button7_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void  on_button8_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void  on_button9_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void on_button10_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void on_button11_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void on_button12_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void on_button13_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void on_button14_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void on_button15_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void on_button16_clicked(GtkButton *b);
extern "C" G_MODULE_EXPORT void  on_toggle1_toggled(GtkButton *b);


void declarations()
{
  fixed1   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"  ));
  button1  = GTK_WIDGET(gtk_builder_get_object(builder, "button1" ));
  button2  = GTK_WIDGET(gtk_builder_get_object(builder, "button2" ));
  button3  = GTK_WIDGET(gtk_builder_get_object(builder, "button3" ));
  entry1   = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"  ));
  entry2   = GTK_WIDGET(gtk_builder_get_object(builder, "entry2"  ));
  label1   = GTK_WIDGET(gtk_builder_get_object(builder, "label1"  ));
  label2   = GTK_WIDGET(gtk_builder_get_object(builder, "label2"  ));
  label3   = GTK_WIDGET(gtk_builder_get_object(builder, "label3"  ));
  label4   = GTK_WIDGET(gtk_builder_get_object(builder, "label4"  ));
  label5   = GTK_WIDGET(gtk_builder_get_object(builder, "label5"  ));


  fixed2   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed2"  ));
  button4  = GTK_WIDGET(gtk_builder_get_object(builder, "button4" ));
  button5  = GTK_WIDGET(gtk_builder_get_object(builder, "button5" ));
  button6  = GTK_WIDGET(gtk_builder_get_object(builder, "button6" ));
  button7  = GTK_WIDGET(gtk_builder_get_object(builder, "button7" ));
  entry3   = GTK_WIDGET(gtk_builder_get_object(builder, "entry3"  ));
  label6   = GTK_WIDGET(gtk_builder_get_object(builder, "label6"  ));
  label7   = GTK_WIDGET(gtk_builder_get_object(builder, "label7"  ));
  label8   = GTK_WIDGET(gtk_builder_get_object(builder, "label8"  ));
  label9   = GTK_WIDGET(gtk_builder_get_object(builder, "label9"  ));
  label10  = GTK_WIDGET(gtk_builder_get_object(builder, "label10" ));
  label11  = GTK_WIDGET(gtk_builder_get_object(builder, "label11" ));


  fixed3   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed3"  ));
  button8  = GTK_WIDGET(gtk_builder_get_object(builder, "button6" ));
  button9  = GTK_WIDGET(gtk_builder_get_object(builder, "button7" ));
  button10 = GTK_WIDGET(gtk_builder_get_object(builder, "button10"));
  label12  = GTK_WIDGET(gtk_builder_get_object(builder, "label12" ));
  label13  = GTK_WIDGET(gtk_builder_get_object(builder, "label13" ));
  label14  = GTK_WIDGET(gtk_builder_get_object(builder, "label14" ));
  label15  = GTK_WIDGET(gtk_builder_get_object(builder, "label15" ));
  label16  = GTK_WIDGET(gtk_builder_get_object(builder, "label16" ));
  label17  = GTK_WIDGET(gtk_builder_get_object(builder, "label17" ));
  label18  = GTK_WIDGET(gtk_builder_get_object(builder, "label18" ));
  label19  = GTK_WIDGET(gtk_builder_get_object(builder, "label19" ));
  label20  = GTK_WIDGET(gtk_builder_get_object(builder, "label20" ));
  spin1    = GTK_WIDGET(gtk_builder_get_object(builder, "spin1"   ));
  spin2    = GTK_WIDGET(gtk_builder_get_object(builder, "spin2"   ));
  check1   = GTK_WIDGET(gtk_builder_get_object(builder, "check1"  ));
  check2   = GTK_WIDGET(gtk_builder_get_object(builder, "check2"  ));
  check3   = GTK_WIDGET(gtk_builder_get_object(builder, "check3"  ));
  check4   = GTK_WIDGET(gtk_builder_get_object(builder, "check4"  ));
  check5   = GTK_WIDGET(gtk_builder_get_object(builder, "check5"  ));
  check6   = GTK_WIDGET(gtk_builder_get_object(builder, "check6"  ));
  check7   = GTK_WIDGET(gtk_builder_get_object(builder, "check7"  ));


  fixed4   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed4"  ));
  button11 = GTK_WIDGET(gtk_builder_get_object(builder, "button11"));
  button12 = GTK_WIDGET(gtk_builder_get_object(builder, "button12"));
  label21  = GTK_WIDGET(gtk_builder_get_object(builder, "label21" ));
  label22  = GTK_WIDGET(gtk_builder_get_object(builder, "label22" ));
  label23  = GTK_WIDGET(gtk_builder_get_object(builder, "label23" ));
  label24  = GTK_WIDGET(gtk_builder_get_object(builder, "label24" ));
  label25  = GTK_WIDGET(gtk_builder_get_object(builder, "label25" ));
  label26  = GTK_WIDGET(gtk_builder_get_object(builder, "label26" ));
  toggle1  = GTK_WIDGET(gtk_builder_get_object(builder, "toggle1" ));


  fixed5   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed5"  ));
  button13 = GTK_WIDGET(gtk_builder_get_object(builder, "button13"));
  button14 = GTK_WIDGET(gtk_builder_get_object(builder, "button14"));
  label27  = GTK_WIDGET(gtk_builder_get_object(builder, "label27" ));
  label28  = GTK_WIDGET(gtk_builder_get_object(builder, "label28" ));
  label29  = GTK_WIDGET(gtk_builder_get_object(builder, "label29" ));
  label30  = GTK_WIDGET(gtk_builder_get_object(builder, "label30" ));
  label31  = GTK_WIDGET(gtk_builder_get_object(builder, "label31" ));
  label32  = GTK_WIDGET(gtk_builder_get_object(builder, "label32" ));
  label33  = GTK_WIDGET(gtk_builder_get_object(builder, "label33" ));
  label34  = GTK_WIDGET(gtk_builder_get_object(builder, "label34" ));
  label37  = GTK_WIDGET(gtk_builder_get_object(builder, "label37" ));
  radio1   = GTK_WIDGET(gtk_builder_get_object(builder, "radio1"  ));
  radio2   = GTK_WIDGET(gtk_builder_get_object(builder, "radio2"  ));
  radio3   = GTK_WIDGET(gtk_builder_get_object(builder, "radio3"  ));


  fixed6   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed6"  ));
  button15 = GTK_WIDGET(gtk_builder_get_object(builder, "button15"));
  label35  = GTK_WIDGET(gtk_builder_get_object(builder, "label35" ));
  label36  = GTK_WIDGET(gtk_builder_get_object(builder, "label36" ));
  label38  = GTK_WIDGET(gtk_builder_get_object(builder, "label38" ));
  label39  = GTK_WIDGET(gtk_builder_get_object(builder, "label39" ));


  fixed7   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed7"  ));
  button16 = GTK_WIDGET(gtk_builder_get_object(builder, "button16"));
  label40  = GTK_WIDGET(gtk_builder_get_object(builder, "label40" ));
  label41  = GTK_WIDGET(gtk_builder_get_object(builder, "label41" ));
  label42  = GTK_WIDGET(gtk_builder_get_object(builder, "label42" ));
  label43  = GTK_WIDGET(gtk_builder_get_object(builder, "label43" ));
  label44  = GTK_WIDGET(gtk_builder_get_object(builder, "label44" ));
  label45  = GTK_WIDGET(gtk_builder_get_object(builder, "label45" ));
  label46  = GTK_WIDGET(gtk_builder_get_object(builder, "label46" ));
  label47  = GTK_WIDGET(gtk_builder_get_object(builder, "label47" ));


  window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
  window2 = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
  window3 = GTK_WIDGET(gtk_builder_get_object(builder, "window3"));
  window4 = GTK_WIDGET(gtk_builder_get_object(builder, "window4"));
  window5 = GTK_WIDGET(gtk_builder_get_object(builder, "window5"));
  window6 = GTK_WIDGET(gtk_builder_get_object(builder, "window6"));
  window7 = GTK_WIDGET(gtk_builder_get_object(builder, "window7"));


  gtk_window_set_title(GTK_WINDOW(window1), "Quizz Game");
  gtk_window_set_title(GTK_WINDOW(window2), "Quizz Game");
  gtk_window_set_title(GTK_WINDOW(window3), "Quizz Game");
  gtk_window_set_title(GTK_WINDOW(window4), "Quizz Game");
  gtk_window_set_title(GTK_WINDOW(window5), "Quizz Game");
  gtk_window_set_title(GTK_WINDOW(window6), "Quizz Game");
  gtk_window_set_decorated(GTK_WINDOW(window7), FALSE);
  gtk_widget_hide(button10);
  gtk_widget_hide(button12);
}