#pragma once

enum class Token {
    Ident,
    Number,

    Skip,
    Read,
    Write,
    If,
    Elif,
    Else,
    While,
    Do,
    For,

    LParen,
    RParen,
    LBrace,
    RBrace,
    Semicolon,
    Assign,

    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Equal,
    NotEqual,
    And,
    Or,

    PlusAssign,
    MinusAssign,
    StarAssign,
    SlashAssign,
    PercentAssign,

    EndOfFile,
};