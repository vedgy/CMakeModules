/*
 This file is part of vedgTools/IncludeExpander.
 Copyright (C) 2014 Igor Kushnir <igorkuo AT Google mail>

 vedgTools/IncludeExpander is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 vedgTools/IncludeExpander is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 vedgTools/IncludeExpander.  If not, see <http://www.gnu.org/licenses/>.
*/

# ifndef PATTERN_UTILITIES_HPP
# define PATTERN_UTILITIES_HPP

# include <cstddef>
# include <cctype>
# include <utility>
# include <functional>
# include <algorithm>
# include <vector>
# include <string>


# define GCC_EARLIER_THAN_4_8 (__GNUC__ == 4 && __GNUC_MINOR__ < 8)
# define NO_INHERITING_CONSTRUCTORS GCC_EARLIER_THAN_4_8


namespace PatternUtilities
{
template <typename UnaryCharPredicate>
inline void skipIf(const std::string & str, std::size_t & index,
                   UnaryCharPredicate f)
{
    index = std::find_if_not(str.begin() + index, str.end(), f) - str.begin();
}

inline void skipWs(const std::string & str, std::size_t & index)
{
    skipIf(str, index, [](char c) { return std::isspace(c); });
}

inline void skipBlank(const std::string & str, std::size_t & index)
{
    skipIf(str, index, [](char c) { return std::isblank(c); });
}

inline void noSkip(const std::string &, std::size_t &)
{}



class Pattern
{
public:
    /// @brief Searches this pattern in the source string.
    /// @param index Specifies starting search position in source.\n In case of
    /// match index will point to the position in source after last matched
    /// symbol. Otherwise, index's final value is defined by derived classes.
    /// @return true in case of match, false otherwise.
    virtual bool match(const std::string & source, std::size_t & index) = 0;

    virtual ~Pattern() {}
};


class SkippingPattern : public Pattern
{
public:
    typedef std::function<void(const std::string & str, std::size_t & index)>
    Discarder;

    explicit SkippingPattern(Discarder discarder = noSkip)
        : discarder_(std::move(discarder)) {}

    /// @brief First discarder_(source, index) is called - it can change index.
    /// Then, if X symbols of source starting from index match this pattern,
    /// index becomes equal to index + X, otherwise, index is not changed on
    /// this stage.
    // bool match(...) override = 0;

protected:
    const Discarder discarder_;
};


class Whitespace : public Pattern
{
public:
    /// @brief Matches if source[index] is a whitespace.\n
    /// If matched, index is incremented, otherwise, index is not changed.
    bool match(const std::string & source, std::size_t & index) override;
};


template<class CharComparator>
class GenericString : public SkippingPattern
{
public:
    explicit GenericString(std::string str, Discarder discarder = noSkip)
        : SkippingPattern(std::move(discarder)), str_(std::move(str)) {}

    /// @brief Matches str_.
    bool match(const std::string & source, std::size_t & index) override {
        discarder_(source, index);
        if (index + str_.size() <= source.size() &&
                std::equal(str_.begin(), str_.end(), source.begin() + index,
                           CharComparator())) {
            index += str_.size();
            return true;
        }
        return false;
    }

private:
    const std::string str_;
};

typedef GenericString<std::equal_to<char>> String;

struct LowerMixedCaseCiCharComparator {
    bool operator()(char lower, char mixed) const {
        return lower == std::tolower(mixed);
    }
};
/// Case-insensitive String.
/// WARNING: CiString constructor parameter 'str' must be in lowercase.
typedef GenericString<LowerMixedCaseCiCharComparator> CiString;


class Param : public SkippingPattern
{
public:
# if NO_INHERITING_CONSTRUCTORS
    explicit Param(Discarder discarder = noSkip)
        : SkippingPattern(std::move(discarder)) {}
# else
    using SkippingPattern::SkippingPattern;
# endif

    /// @brief Matches any sequence of non-whitespace and not ')' characters.
    bool match(const std::string & source, std::size_t & index) override;

    /// @return Last matched param. If match has never occured, empty string.
    const std::string & getParam() const { return param_; }

private:
    std::string param_;
};


class ParamCopy : public SkippingPattern
{
public:
    explicit ParamCopy(const Param & param, Discarder discarder = noSkip)
        : SkippingPattern(std::move(discarder)), param_(param) {}

    bool match(const std::string & source, std::size_t & index) override {
        return String(param_.getParam(), discarder_).match(source, index);
    }

private:
    const Param & param_;
};


class SearchSymbol : public Pattern
{
public:
    explicit SearchSymbol(char symbol) : symbol_(symbol) {}

    /// @brief Finds symbol_. Skips everything before symbol_.
    /// @param index If symbol_ was found, points to the position in source
    /// right after the position of symbol_. Otherwise, index's resulting value
    /// is std::string::npos.
    bool match(const std::string & source, std::size_t & index) override;

    /// @return Position of the found symbol_ in source. Undefined
    /// number if match has never occured.
    std::size_t getSymbolPosition() const { return symbolPosition_; }

private:
    const char symbol_;
    std::size_t symbolPosition_;
};


class SearchLine : public Pattern
{
public:
    /// @brief Finds line, which matches this pattern. Skips everything
    /// before this line. Pattern from derived classes is considered to be found
    /// only if it starts the line (i.e. there are no non-whitespace symbols in
    /// the line before value).
    /// @param index If line was found, points to first symbol in source after
    /// the last matched symbol (not necessarily last symbol of the line).
    /// Otherwise, index's resulting value is std::string::npos.
    bool match(const std::string & source, std::size_t & index) override;

    /// @return Position of the beginning of matched line in source. Undefined
    /// number if match has never occured.
    std::size_t getLineBeginning() const { return lineBeginning_; }

    /// @return Position of the beginning of matched pattern in source.
    /// Undefined number if match has never occured.
    std::size_t getPatternBeginning() const { return patternBeginning_; }

private:
    /// @brief Finds desired pattern. Is used in match().
    /// @param index Is set to position in source of the first symbol of matched
    /// pattern. If pattern wasn't found, is set to std::string::npos.
    virtual void findStr(const std::string & source, std::size_t & index) = 0;

    /// @brief Is used in match().
    /// @return Length of matched pattern.
    virtual std::size_t size() const = 0;

    std::size_t lineBeginning_, patternBeginning_;
};


class SearchStringLine : public SearchLine
{
public:
    explicit SearchStringLine(std::string str) : str_(std::move(str)) {}

private:
    void findStr(const std::string & source, std::size_t & index) override {
        index = source.find(str_, index);
    }

    std::size_t size() const override { return str_.size(); }

    const std::string str_;
};


class SearchCiStringLine : public SearchLine
{
public:
    /// WARNING: lowerStr must be in lowercase!
    explicit SearchCiStringLine(const std::string & lowerStr);

private:
    void findStr(const std::string & source, std::size_t & index) override;

    std::size_t size() const override {
        return lowerStrWithoutFirstSymbol_.size() + 1;
    }

    /// Stores 2 symbols: both lower- and uppercase first symbol versions.
    /// If first symbol of lowerStr wasn't letter, stores only one symbol.
    /// NOTE: non-const only because of verbose initialization in constructor.
    /// Must not be changed after constructor.
    std::string firstSymbol_;
    const std::string lowerStrWithoutFirstSymbol_;
};



/// @brief Matches source string with the sequence of patterns.
class PatternMatcher
{
public:
    typedef std::vector<Pattern *> PatternSequence;

    explicit PatternMatcher(const PatternSequence & patternSequence)
        : patternSequence_(patternSequence)
    {}

    std::size_t currentPatternIndex() const { return patternId_; }

    /// @brief Resets current pattern id to initial state. Thus, enables reuse.
    void reset() { patternId_ = 0; }

    /// @param index Specifies starting search position in source.\n
    /// In case of false return value, index is set to the position
    /// in source of the first mismatched symbol.\n
    /// In case of true return value, index's value is determined by first
    /// mismatched pattern (i.e. position in source right after the last matched
    /// symbol for most patterns; and std::string::npos in case of
    /// Search* patterns).
    /// @return true if source matches patternSequence_.
    bool match(const std::string & source, std::size_t & index);

private:
    const PatternSequence & patternSequence_;
    std::size_t patternId_ = 0;
};

}

# endif // PATTERN_UTILITIES_HPP
