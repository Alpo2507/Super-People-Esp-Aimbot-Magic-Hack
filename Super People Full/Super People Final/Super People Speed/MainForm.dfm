object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'kojd'
  ClientHeight = 97
  ClientWidth = 130
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 31
    Height = 13
    Caption = 'boost:'
  end
  object Label2: TLabel
    Left = 8
    Top = 27
    Width = 30
    Height = 13
    Caption = 'delay:'
  end
  object Label3: TLabel
    Left = 75
    Top = 31
    Width = 52
    Height = 13
    Caption = '[50 ~ 100]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 75
    Top = 8
    Width = 40
    Height = 13
    Caption = '[5 ~ 10]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object txtboost: TEdit
    Left = 44
    Top = 8
    Width = 25
    Height = 21
    TabOrder = 0
    Text = '5'
  end
  object txtdelay: TEdit
    Left = 44
    Top = 27
    Width = 25
    Height = 21
    TabOrder = 1
    Text = '50'
  end
  object chwh: TCheckBox
    Left = 8
    Top = 50
    Width = 61
    Height = 17
    Caption = 'wallhack'
    TabOrder = 2
    OnClick = chwhClick
  end
  object chenabled: TCheckBox
    Left = 8
    Top = 73
    Width = 65
    Height = 17
    Caption = 'enabled'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    OnClick = chenabledClick
  end
end
