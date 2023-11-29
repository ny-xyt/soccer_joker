/*=============================================================================
    Copyright (c) 2002-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See below or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

/*
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


////////////////////////////////////////////////////////////////////////////
//
//  Plain SkillParser example demostrating the grammar and semantic actions.
//  This is discussed in the "Grammar" and "Semantic Actions" chapters in
//  the Spirit User's Guide.
//
//  [ JDG 5/10/2002 ]
//
////////////////////////////////////////////////////////////////////////////
//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/include/classic_confix.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/spirit/include/classic_clear_actor.hpp>
#include <boost/spirit/include/classic_push_back_actor.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "skill.h"

////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace BOOST_SPIRIT_CLASSIC_NS;
using namespace boost::spirit::classic;
using namespace phoenix;



typedef char                    char_t;
typedef file_iterator <char_t>  iterator_t;

////////////////////////////////////////////////////////////////////////////
//
//  Our SkillParser grammar
//
////////////////////////////////////////////////////////////////////////////

// What is it? see: http://www.boost.org/doc/libs/1_38_0/libs/spirit/classic/example/fundamental/phoenix_calc.cpp
struct calc_closure : BOOST_SPIRIT_CLASSIC_NS::closure<calc_closure, double>
{
    member1 val;
};


struct SkillParser : public grammar<SkillParser>
{
    SkillParser(map< SkillType, boost::shared_ptr<Skill> >& skills_, BodyModel* bodyModel_)
        : skills( skills_ ), bodyModel( bodyModel_ )
    {}

    template <typename ScannerT>
    struct definition
    {
        definition(SkillParser const& self)
        {
            top
            =   +( skill | reflectskill )   ;



            skill
            =   str_p("STARTSKILL")          >>
                ID[ assign_a( skillname ) ][ startSkillCB( skillname, currentSkillType, self ) ]  >>
                +(state)               >>
                str_p("ENDSKILL")           ;// >>
//                    ID                          ;
            // TODO: cannot get parse_info.full here. Why?



            reflectskill
            =   ( str_p("REFLECTSKILL")             >>
                  ID [ assign_a( refSkillSource ) ] >>
                  ID [ assign_a( refSkillTarget ) ]
                )
                [ reflectSkillCB( refSkillSource, refSkillTarget, self ) ] ;




            state
            =   str_p("STARTSTATE") [ startKeyFrameCB( currentSkillType, keyFrame, self ) ]          >>
                +macro                       >>
                str_p("ENDSTATE")            ;



            macro
            =   reset | inctar | wait | settar | setfoot | setscale | curve | stabilize;


            reset
            =   confix_p( /* a triple: (open, expression, close) */
                    str_p("reset") [clear_a(jointsToReset)],

                    // push each joint into the jointsToReset container
                    +( ID [ push_back_a( jointsToReset ) ] ),

                    str_p("end")
                )
                // Do something with all the joints
                [ resetCB( jointsToReset, keyFrame, self ) ]
                ;



            inctar
            =   confix_p( str_p("inctar") [clear_a( effectorsToIncrease )] [clear_a( effectorsToIncValues )],
                          +( ID [ push_back_a( effectorsToIncrease ) ]
                             >> VALUE [ push_back_a( effectorsToIncValues, tmp ) ]
                           ),
                          str_p("end")
                        )
                // Do something with all the joints
                [ inctarCB( effectorsToIncrease,
                            effectorsToIncValues,
                            keyFrame,
                            self ) ]
                ;


            wait
            =   confix_p( str_p("wait"),
                          VALUE [ assign_a( waitTime, tmp ) ],
                          str_p("end")
                        )
                [ waitCB( waitTime, keyFrame, self ) ]
                ;




            settar
            =   confix_p( str_p("settar") [clear_a( effectorsToSet )] [clear_a( effectorsToSetValues )],
                          +( ID [ push_back_a( effectorsToSet ) ]
                             >> VALUE [ push_back_a( effectorsToSetValues, tmp ) ]
                           ),
                          str_p("end")
                        )
                // Do something with all the joints
                [ settarCB( effectorsToSet,
                            effectorsToSetValues,
                            keyFrame,
                            self ) ]
                ;



            setfoot
            =  confix_p(str_p("setfoot") [ clear_a( pos ) ],
                        (
                            ID [ assign_a( footToSet ) ]  >>
                            VALUE [ push_back_a (pos, tmp) ]   >>
                            VALUE [ push_back_a (pos, tmp) ]   >>
                            VALUE [ push_back_a (pos, tmp) ]   >>
                            VALUE [ push_back_a (pos, tmp) ]   >>
                            VALUE [ push_back_a (pos, tmp) ]   >>
                            VALUE [ push_back_a (pos, tmp) ]
                        ),
                        str_p("end")
                       )
               [ setfootCB( footToSet, pos, keyFrame, self ) ]
               ;




            setscale
            =   confix_p( str_p("setscale") [clear_a( effectorsToScale )] [clear_a( effectorsToScaleValues )],
                          +( ID [ push_back_a( effectorsToScale ) ]
                             >> VALUE [ push_back_a( effectorsToScaleValues, tmp ) ]
                           ),
                          str_p("end")
                        )
                // Do something with all the joints
                [ setscaleCB( effectorsToScale,
                              effectorsToScaleValues,
                              keyFrame,
                              self ) ]
                ;



            curve
            =    confix_p( str_p("STARTCURVE") [ clear_a(pos) ] >>
                           ID [ assign_a( footToSet ) ],
                           (
                               +controlpoint
                           ),
                           str_p("ENDCURVE"))
                 [ curveCB( footToSet, pos, keyFrame, self ) ]
                 ;



            controlpoint
            =   confix_p( str_p("controlpoint"),
                          (
                              VALUE [ push_back_a (pos, tmp) ]   >>
                              VALUE [ push_back_a (pos, tmp) ]   >>
                              VALUE [ push_back_a (pos, tmp) ]   >>
                              VALUE [ push_back_a (pos, tmp) ]   >>
                              VALUE [ push_back_a (pos, tmp) ]   >>
                              VALUE [ push_back_a (pos, tmp) ]
                          ) ,
                          str_p("end"));

            stabilize
            =  confix_p(str_p("stabilize") [ clear_a( pos ) ],
                        (
                            ID [ assign_a( footToSet ) ] >>
                            VALUE [ push_back_a (pos, tmp) ] >>
                            VALUE [ push_back_a (pos, tmp) ]
                        ),
                        str_p("end")
                       )
               [ stabilizeCB( footToSet, pos, keyFrame, self ) ]
               ;



            ID
            =   lexeme_d[ +(alnum_p | ch_p('_')) ] - str_p("end"); // end is not a legal id




            // From here, taken from:
            // http://www.boost.org/doc/libs/1_38_0/libs/spirit/classic/example/fundamental/phoenix_calc.cpp
            // No idea how it works, but what important is that tmp would hold the final expression value...
            VALUE
            =   expression[ var(tmp) = arg1 ];




            expression
            =   term[expression.val = arg1]
                >> *(   ('+' >> term[expression.val += arg1])
                        |   ('-' >> term[expression.val -= arg1])
                    )
                ;



            term
            =   factor[term.val = arg1]
                >> *(   ('*' >> factor[term.val *= arg1])
                        |   ('/' >> factor[term.val /= arg1])
                    )
                ;



            factor
            =   ureal_p[factor.val = arg1]
                |   '(' >> expression[factor.val = arg1] >> ')'
                |   ('-' >> factor[factor.val = -arg1])
                |   ('+' >> factor[factor.val = arg1])
                ;






            /////////////////////////////////////////////////////
            // This macros should be enabled when debugging
            // More like these could be defined for each rule
            //////////////////////////////////////////////////
            /*
                        BOOST_SPIRIT_DEBUG_NODE(skill);
                        BOOST_SPIRIT_DEBUG_NODE(reflectskill);
                        BOOST_SPIRIT_DEBUG_NODE(state);
                        BOOST_SPIRIT_DEBUG_NODE(macro);
                        BOOST_SPIRIT_DEBUG_NODE(reset);
                        BOOST_SPIRIT_DEBUG_NODE(inctar);
                        BOOST_SPIRIT_DEBUG_NODE(wait);
                        BOOST_SPIRIT_DEBUG_NODE(settar);
                        BOOST_SPIRIT_DEBUG_NODE(ID);
            */

        }

        // variables
        double waitTime;
        string skillname;
        string refSkillSource;
        string refSkillTarget;
        SkillType currentSkillType;
        vector<string> jointsToReset;
        vector<string> effectorsToIncrease;
        vector<double> effectorsToIncValues;
        vector<string> effectorsToSet;
        vector<double> effectorsToSetValues;
        string footToSet;
        VecPosition footToSetXyz, footToSetRpy;
        vector<string> effectorsToScale;
        vector<double> effectorsToScaleValues;
        boost::shared_ptr< KeyFrame > keyFrame;
        double tmp;
        vector<double> pos;

        // rules

        rule<ScannerT> top, skill, reflectskill, state, curve, macro, reset,
             inctar, wait, settar, setfoot, setscale, controlpoint, stabilize;
        rule<ScannerT> ID;
        rule<ScannerT> VALUE;

        typedef rule<ScannerT, calc_closure::context_t> rule_t;
        rule_t expression, term, factor;

        rule<ScannerT> const&
        start() const {
            return top;
        }

        /*
         * -----------------------------------------------------------------------
         *  Above here is the standard code from the boost website
         *  Below is the callbacks used during the parsing.
         *  I put them inside the class because the grammar should know about them
         *  and they should know about the grammar so it's not recommended to
         *  move them outside...
         *  Daniel
         */


        //////////////////////////////////////////
        // Functors
        /////////////////////////////////////////

        struct vec3CB {

            vec3CB(double &tmp, vector<VecPosition> &positions, VecPosition &tmpVec) :
                tmp_(tmp), positions_(positions), tmpVec_(tmpVec) {
            }

            void operator()(char const* str, char const* end) const {
                this->operator()(iterator_t(), iterator_t());
            }

            void operator()(iterator_t, iterator_t) const {
                LOG(tmp_);
                LOG(positions_);
                LOG(tmpVec_);
                tmpVec_ = positions_[0];
                LOG(tmpVec_);
            }

            double &tmp_;
            vector<VecPosition> &positions_;
            VecPosition &tmpVec_;
        };

        struct resetCB {

            resetCB( vector<string>& joints_, boost::shared_ptr< KeyFrame >& keyFrame_, SkillParser const& parser_ )
                : joints( joints_ ), keyFrame( keyFrame_ ), parser(parser_) {
            }

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {

                // Translate joint string names to enums
                vector<string>::const_iterator begin( joints.begin() ), end( joints.end() );
                vector<int> jointIndices;
//              cerr << "Reset joints: ";
                for( ; begin != end; ++begin ) {
//                cerr << *begin << ' ' <<  enumParser.getEnumFromString( *begin );
                    // translate str->enum-index
                    jointIndices.push_back( EnumParser<BodyParts>::getEnumFromString( *begin ) );
                }

                boost::shared_ptr< Macro > resetMacro( new Reset( jointIndices ) );
                keyFrame->appendMacro( resetMacro );
            }

            // functor variables, recommended to be references
            vector<string>& joints;
            boost::shared_ptr< KeyFrame >& keyFrame;
            SkillParser const &parser;
//          EnumParser<BodyParts> &enumParser;
        };






        struct inctarCB {

            inctarCB( vector<string>& effectors_, vector<double>& effValues_, boost::shared_ptr< KeyFrame >& keyFrame_, SkillParser const& parser_ )
                : effectors(effectors_),
                  effValues(effValues_),
                  keyFrame( keyFrame_ ),
                  parser(parser_) {
            }

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {

                // Translate effector string names to enums
                vector<int> effIndices;
//              cerr << "inctar joints: ";
                for(unsigned i = 0; i < effectors.size(); ++i ) {
//                cerr << effectors[i] << '(' << enumParser.getEnumFromString( effectors[i] ) << ')'
//                     << '=' << effValues[i] << endl;
                    // translate str->enum-index
                    effIndices.push_back( EnumParser<Effectors>::getEnumFromString( effectors[i] ) );
                }

                boost::shared_ptr< Macro > inctarMacro( new IncTar( effIndices, effValues ) );//在这里IncTar获得它的关节及其增量的数组
                keyFrame->appendMacro( inctarMacro );//将该动作加到帧的里面，最终存到了maceos数组里面 
		//猜测：对每一个skill都有自己专属的一个macros数组存着自己各个帧的动作，如IncTar。这些动作调用execute()并在里面生成轨迹执行自己的动作
            }

            // functor variables, recommended to be references
            vector<string>& effectors;
            vector<double>& effValues;
            boost::shared_ptr< KeyFrame >& keyFrame;
            SkillParser const &parser;
//          EnumParser<Effectors> enumParser;
        };





        struct setscaleCB {

            setscaleCB( vector<string>& effectors_, vector<double>& effValues_, boost::shared_ptr< KeyFrame >& keyFrame_, SkillParser const& parser_ )
                : effectors(effectors_),
                  effValues(effValues_),
                  keyFrame( keyFrame_ ),
                  parser(parser_) {
            }

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {

                // Translate effector string names to enums
                vector<int> effIndices;
//              cerr << "inctar joints: ";
                for(unsigned i = 0; i < effectors.size(); ++i ) {
//                cerr << effectors[i] << '(' << enumParser.getEnumFromString( effectors[i] ) << ')'
//                     << '=' << effValues[i] << endl;
                    // translate str->enum-index
                    effIndices.push_back( EnumParser<Effectors>::getEnumFromString( effectors[i] ) );
                }

                boost::shared_ptr< Macro > setscaleMacro( new SetScale( effIndices, effValues ) );
                keyFrame->appendMacro( setscaleMacro );
            }

            // functor variables, recommended to be references
            vector<string>& effectors;
            vector<double>& effValues;
            boost::shared_ptr< KeyFrame >& keyFrame;
            SkillParser const &parser;
//          EnumParser<Effectors> enumParser;
        };





        struct settarCB {

            settarCB( vector<string>& effectors_, vector<double>& effValues_, boost::shared_ptr< KeyFrame >& keyFrame_, SkillParser const& parser_ )
                : effectors(effectors_),
                  effValues(effValues_),
                  keyFrame( keyFrame_ ),
                  parser(parser_) {
            }

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {

                // Translate effector string names to enums
                vector<int> effIndices;
//              cerr << "inctar joints: ";
                for(unsigned i = 0; i < effectors.size(); ++i ) {
//                cerr << effectors[i] << '(' << enumParser.getEnumFromString( effectors[i] ) << ')'
//                     << '=' << effValues[i] << endl;
                    // translate str->enum-index
                    effIndices.push_back( EnumParser<Effectors>::getEnumFromString( effectors[i] ) );
                }

                boost::shared_ptr< Macro > settarMacro( new SetTar( effIndices, effValues ) );
                keyFrame->appendMacro( settarMacro );
            }

            // functor variables, recommended to be references
            vector<string>& effectors;
            vector<double>& effValues;
            boost::shared_ptr< KeyFrame >& keyFrame;
            SkillParser const &parser;
//          EnumParser<Effectors> enumParser;
        };





        struct setfootCB {

            setfootCB( string& foot_, vector<double> &pos_, boost::shared_ptr< KeyFrame >& keyFrame_, SkillParser const& parser_ )
                : foot(foot_),
                  pos(pos_),
                  keyFrame( keyFrame_ ),
                  parser(parser_) {
            }

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {
                // Translate effector string names to enums
                int footIDX = EnumParser<BodyParts>::getEnumFromString( foot );
                VecPosition xyz(pos[0], pos[1], pos[2]);
                VecPosition rpy(pos[3], pos[4], pos[5]);
                // TODO: populate xyz and rpy
                Pos6DOF pos6dof = { xyz, rpy };

//              LOG(foot);
//              LOG(footIDX);
//              LOG(pos);
//              LOG(xyz);
//              LOG(rpy);
//              LOG(pos6dof);

                boost::shared_ptr< Macro > setfootMacro( new SetFoot( footIDX, pos6dof ) );
                keyFrame->appendMacro( setfootMacro );
            }

            // functor variables, recommended to be references
            string &foot;
            vector<double> &pos;
            boost::shared_ptr< KeyFrame > &keyFrame;
            SkillParser const &parser;
//          EnumParser<BodyParts> enumParser;
        };


        struct stabilizeCB {
            stabilizeCB( string& foot_, vector<double> &pos_, boost::shared_ptr< KeyFrame >& keyFrame_, SkillParser const& parser_ )
                : foot(foot_),
                  pos(pos_),
                  keyFrame( keyFrame_ ),
                  parser(parser_) {}

            void operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void operator() ( iterator_t, iterator_t ) const {
                // Translate effector string names to enums
                int footIndex = EnumParser<BodyParts>::getEnumFromString( foot );
                VecPosition zmp(pos[0], pos[1], 0);
                boost::shared_ptr< Macro > stabilizeMacro(new Stabilize(footIndex, zmp));
                keyFrame->appendMacro(stabilizeMacro);
            }

            // functor variables, recommended to be references
            string &foot;
            vector<double> &pos;
            boost::shared_ptr< KeyFrame > &keyFrame;
            SkillParser const &parser;
        };




        struct waitCB {

            waitCB( double& waitTime_, boost::shared_ptr< KeyFrame >& keyFrame_, SkillParser const& parser_ )
                : waitTime( waitTime_ ), keyFrame( keyFrame_ ), parser(parser_) {
            }

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {

//              cerr << "waitCB(), wait time is " <<  waitTime + 0.0 << endl;

                keyFrame->setToWaitTime( true );
                keyFrame->setWaitTime( waitTime );

            }

            // functor variables, recommended to be references
            double& waitTime;
            boost::shared_ptr< KeyFrame >& keyFrame;
            SkillParser const &parser;
        };





        struct curveCB {

            curveCB( const string &foot_, const vector<double> &pos_,
                     boost::shared_ptr< KeyFrame > &keyFrame_, SkillParser const &parser_ )
                : foot(foot_), pos(pos_),
                  keyFrame(keyFrame_), parser(parser_) {
            }

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {
                int footIDX = EnumParser<BodyParts>::getEnumFromString( foot );

                vector<Pos6DOF> controlPoints(pos.size() / 6);
                for(size_t i = 0; i < pos.size(); i += 6) {
                    VecPosition xyz(pos[i], pos[i+1], pos[i+2]);
                    VecPosition rpy(pos[i+3], pos[i+4], pos[i+5]);
                    controlPoints[i/6].xyz = xyz;
                    controlPoints[i/6].rpy = rpy;
                }

                boost::shared_ptr< Macro > curveMacro( new Curve( footIDX, controlPoints ) );
                keyFrame->appendMacro( curveMacro );
            }

            // functor variables, recommended to be references
            const string &foot;
            const vector<double> &pos;
            boost::shared_ptr< KeyFrame > &keyFrame;
            SkillParser const &parser;
//          EnumParser<BodyParts> enumParser;
        };






        /*
         * Assuming skillname_ is already initialized, we allocate new skill and
         * initialize currentSkillType
         */
        struct startSkillCB {

            startSkillCB( std::string& skillname_, SkillType& currentSkillType_, SkillParser const& parser_ )
                : skillname( skillname_ ), currentSkillType( currentSkillType_ ), parser( parser_ ) {
            }

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {
                currentSkillType = EnumParser<SkillType>::getEnumFromString( skillname );
                parser.skills[currentSkillType] =  boost::shared_ptr<Skill>( new Skill() );
            }

            std::string& skillname;
            SkillType& currentSkillType;
            SkillParser const &parser;
//          EnumParser<SkillType> enumParser;
        };





        /*
         * reflect from source to target. Source must exist in memory!
         */
        struct reflectSkillCB {

            reflectSkillCB( std::string& refSkillSource_, std::string& refSkillTarget_, SkillParser const& parser_ )
                : refSkillSource( refSkillSource_ ), refSkillTarget( refSkillTarget_ ), parser( parser_ ) {
            }

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {
                SkillType source = EnumParser<SkillType>::getEnumFromString( refSkillSource );
                SkillType target = EnumParser<SkillType>::getEnumFromString( refSkillTarget );
                parser.skills[target] = parser.skills[source]->getReflection( parser.bodyModel );
            }

            std::string& refSkillSource;
            std::string& refSkillTarget;
            SkillParser const &parser;
//          EnumParser<SkillType> enumParser;
        };




        /*
         * Assuming currentSkillType is initialized, we allocate new frame
         * and append it to the current skill
         */
        struct startKeyFrameCB {

            startKeyFrameCB( SkillType& currentSkillType_,
                             boost::shared_ptr< KeyFrame >& keyFrame_,
                             SkillParser const& parser_ )
                : currentSkillType(currentSkillType_), keyFrame(keyFrame_), parser(parser_)
            {}

            void
            operator() ( char const* str, char const* end ) const {
                this->operator() ( iterator_t(), iterator_t() );
            }

            void
            operator() ( iterator_t, iterator_t ) const {
                keyFrame.reset( new KeyFrame() );
                boost::shared_ptr<Skill> currentSkill = parser.skills[currentSkillType];
                currentSkill->appendKeyFrame( keyFrame );
            }

            // functor variables, recommended to be references
            SkillType& currentSkillType;
            boost::shared_ptr< KeyFrame >& keyFrame;
            SkillParser const &parser;
        };


    };

    // Data members of the grammar - sent in the constructor and used during parsing
    map< SkillType, boost::shared_ptr<Skill> >& skills;
    BodyModel* bodyModel;
};

////////////////////////////////////////////////////////////////////////////
//
//  Main program
//
////////////////////////////////////////////////////////////////////////////
//int
//main()
//{
//    //cout << "/////////////////////////////////////////////////////////\n\n";
//    //cout << "\t\tSkills parser...\n\n";
//    //cout << "/////////////////////////////////////////////////////////\n\n";
//    //cout << "Type an filename to parse...or [q or Q] to quit\n\n";
//
//    // TODO: replace sent parameter to skills structure
//    SkillParser parser(new int[3]);    //  Our parser
//    string str;
//    while (getline(cin, str))
//    {
//        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
//            break;
//
//        string filename = str;
//
//        iterator_t first(filename.c_str());
//        if (!first)
//        {
//          std:://cout << "Unable to open file!\n";
//          return -1;
//        } else {
//          std:://cout << "Opened file" << endl;
//        }
//        iterator_t last = first.make_end();
//
//        parse_info<iterator_t> info = parse(first, last,
//                                            parser,
//                                            ( space_p | comment_p("#") )
//                                            );
//        // TODO: cannot get full here
//        if (info.hit)
//        {
//            //cout << "-------------------------\n";
//            //cout << "Parsing succeeded\n";
//            //cout << "-------------------------\n";
//            //cout << "stop "  << info.stop << endl;
//            //cout << "full " << info.full << endl;
//            //cout << "length " << info.length << endl;
//        }
//        else
//        {
//            //cout << "-------------------------\n";
//            //cout << "Parsing failed\n";
////            //cout << "stopped at: \": " << info.stop << "\"\n";
//            //cout << "-------------------------\n";
//        }
//    }
//
//    //cout << "Bye... :-) \n\n";
//    return 0;
//}
//
//
//