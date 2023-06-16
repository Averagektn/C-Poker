object InfoForm: TInfoForm
  Left = 715
  Top = 291
  Width = 749
  Height = 480
  VertScrollBar.ButtonSize = 10
  VertScrollBar.Position = 239
  VertScrollBar.Range = 680
  VertScrollBar.Size = 10
  VertScrollBar.Tracking = True
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Info'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  TextHeight = 15
  object memInfo: TMemo
    Left = 0
    Top = -239
    Width = 716
    Height = 680
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Segoe UI'
    Font.Style = []
    Lines.Strings = (
      
        'Texas holdem is one of the most popular variants of the card gam' +
        'e of poker. '
      
        'Two cards, known as hole cards, are dealt face down to each play' +
        'er, and then five '
      'community cards are dealt face up in three stages.'
      
        'The stages consist of a series of three cards ("the flop"), late' +
        'r an additional single '
      'card ("the turn"), and a final card ("the river").'
      
        'Each player seeks the best five card poker hand from any combina' +
        'tion of the seven '
      'cards; the five community cards and their two hole cards. '
      'Players have betting options to check, call, raise, or fold.'
      
        'Rounds of betting take place before the flop is dealt and after ' +
        'each subsequent deal. '
      
        'The player who has the best hand and has not folded by the end o' +
        'f all betting '
      
        'rounds wins all of the money bet for the hand, known as the pot.' +
        ' '
      
        'In certain situations, a "split-pot" or "tie" can occur when two' +
        ' players have hands of '
      'equivalent value. This is also called a "chop-pot".'
      ''
      
        'These hand rankings aren'#39't specifically part of Texas hold'#39'em ru' +
        'les, but apply to '
      'many different poker games.'
      ''
      
        'Royal Flush '#8212' five cards of the same suit, ranked ace through te' +
        'n'
      
        'Straight Flush '#8212' five cards of the same suit and consecutively r' +
        'anked'
      'Four of a Kind '#8212' four cards of the same rank'
      
        'Full House '#8212' three cards of the same rank and two more cards of ' +
        'the same rank'
      'Flush '#8212' any five cards of the same suit'
      'Straight '#8212' any five cards consecutively ranked'
      'Three of a Kind '#8212' three cards of the same rank'
      
        'Two Pair '#8212' two cards of the same rank and two more cards of the ' +
        'same rank'
      'One Pair '#8212' two cards of the same rank'
      'High Card '#8212' unmatched cards')
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
  end
end
