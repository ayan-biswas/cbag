#include <cbagoa/database.h>
#include <iostream>

namespace cbagoa {

    cbag::Orientation convert_orient(const oa::oaOrient &orient) {
        switch (orient) {
            case oa::oacR0:
                return cbag::Orientation::R0;
            case oa::oacR90:
                return cbag::Orientation::R90;
            case oa::oacR180:
                return cbag::Orientation::R180;
            case oa::oacR270:
                return cbag::Orientation::R270;
            case oa::oacMY:
                return cbag::Orientation::MY;
            case oa::oacMYR90:
                return cbag::Orientation::MYR90;
            case oa::oacMX:
                return cbag::Orientation::MX;
            case oa::oacMXR90:
                return cbag::Orientation::MXR90;
            default:
                throw std::invalid_argument("Unknown orientation code.");
        }
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

    oa::oaBoolean LibDefObserver::onLoadWarnings(oa::oaLibDefList *obj, const oa::oaString &msg,
                                                 oa::oaLibDefListWarningTypeEnum type) {
        throw std::runtime_error("OA Error: " + std::string(msg));
    }

#pragma clang diagnostic pop

    Library::~Library() {
        close();
    }

    void Library::open_lib(const std::string &lib_file, const std::string &library,
                           const std::string &lib_path, const std::string &tech_lib) {
        try {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "NotAssignable"
            oaDesignInit(oacAPIMajorRevNumber, oacAPIMinorRevNumber, // NOLINT
                         oacDataModelRevNumber); // NOLINT
#pragma clang diagnostic pop

            // open library definition
            oa::oaString lib_def_file(lib_file.c_str());
            oa::oaLibDefList::openLibs(lib_def_file);

            // open library
            lib_name = library;
            lib_name_oa = oa::oaScalarName(ns, library.c_str());
            lib_ptr = oa::oaLib::find(lib_name_oa);
            if (lib_ptr == nullptr) {
                // create new library
                oa::oaString oa_lib_path(lib_path.c_str());
                oa::oaScalarName oa_tech_lib(ns, tech_lib.c_str());
                lib_ptr = oa::oaLib::create(lib_name_oa, oa_lib_path);
                oa::oaTech::attach(lib_ptr, oa_tech_lib);

                // NOTE: I cannot get open access to modify the library file, so
                // we just do it by hand.
                std::ofstream outfile;
                outfile.open(lib_file, std::ios_base::app);
                outfile << "DEFINE " << library << " " << lib_path << std::endl;
            } else if (!lib_ptr->isValid()) {
                throw std::invalid_argument("Invalid library: " + library);
            }

            // open technology file
            tech_ptr = oa::oaTech::find(lib_ptr);
            if (tech_ptr == nullptr) {
                // opened tech not found, attempt to open
                if (!oa::oaTech::exists(lib_ptr)) {
                    throw std::runtime_error("Cannot find technology for library: " + library);
                } else {
                    tech_ptr = oa::oaTech::open(lib_ptr, 'r');
                    if (tech_ptr == nullptr) {
                        throw std::runtime_error("Cannot open technology for library: " + library);
                    }
                }
            }

            // get database unit
            dbu_per_uu = tech_ptr->getDBUPerUU(oa::oaViewType::get(oa::oacMaskLayout));

            is_open = true;
        } catch (oa::oaCompatibilityError &ex) {
            std::string msg_std(ex.getMsg());
            throw std::runtime_error("OA Compatibility Error: " + std::string(ex.getMsg()));
        } catch (oa::oaDMError &ex) {
            throw std::runtime_error("OA DM Error: " + std::string(ex.getMsg()));
        } catch (oa::oaError &ex) {
            throw std::runtime_error("OA Error: " + std::string(ex.getMsg()));
        } catch (oa::oaDesignError &ex) {
            throw std::runtime_error("OA Design Error: " + std::string(ex.getMsg()));
        }
    }

    cbag::CSchMaster Library::parse_sch(const std::string &cell_name,
                                        const std::string &view_name) {
        // get OA Block pointer
        oa::oaScalarName cell_oa(ns, cell_name.c_str());
        oa::oaScalarName view_oa(ns, view_name.c_str());

        oa::oaDesign *dsn_ptr = oa::oaDesign::open(lib_name_oa, cell_oa, view_oa,
                                                   oa::oaViewType::get(oa::oacSchematic), 'r');
        std::ostringstream errstream;
        if (dsn_ptr == nullptr) {
            errstream << "Cannot open cell: " << lib_name << "__" << cell_name
                      << "(" << view_name << ").";
            throw std::invalid_argument(errstream.str());
        }

        oa::oaBlock *blk_ptr = dsn_ptr->getTopBlock();
        if (blk_ptr == nullptr) {
            errstream << "Cannot open top block for cell: " << lib_name << "__" << cell_name
                      << "(" << view_name << ").";
            throw std::invalid_argument(errstream.str());

        }

        // place holder classes
        oa::oaString tmp_str;
        // create schematic master
        cbag::CSchMaster ans;

        // get pins
        oa::oaIter<oa::oaPin> pin_iter(blk_ptr->getPins());
        oa::oaPin *pin_ptr;
        while ((pin_ptr = pin_iter.getNext()) != nullptr) {
            oa::oaTerm *term_ptr = pin_ptr->getTerm();
            oa::oaString tmp_;
            term_ptr->getName(ns_cdba, tmp_str);
            switch (term_ptr->getTermType()) {
                case oa::oacInputTermType :
                    ans.in_pins.push_back(make_name(std::string(tmp_str)));
                    break;
                case oa::oacOutputTermType :
                    ans.out_pins.push_back(make_name(std::string(tmp_str)));
                    break;
                case oa::oacInputOutputTermType :
                    ans.io_pins.push_back(make_name(std::string(tmp_str)));
                    break;
                default:
                    term_ptr->getName(ns_cdba, tmp_str);
                    errstream << "Pin " << tmp_str
                              << " has invalid terminal type: " << term_ptr->getTermType().getName();
                    throw std::invalid_argument(errstream.str());
            }
        }

        // get instances
        oa::oaIter<oa::oaInst> inst_iter(blk_ptr->getInsts());
        oa::oaInst *inst_ptr;
        while ((inst_ptr = inst_iter.getNext()) != nullptr) {
            oa::oaString inst_lib_oa, inst_cell_oa, inst_name_oa;
            inst_ptr->getLibName(ns_cdba, inst_lib_oa);
            inst_ptr->getCellName(ns_cdba, inst_cell_oa);
            // NOTE: exclude pin instances
            if (inst_lib_oa != "basic" ||
                (inst_cell_oa != "ipin" && inst_cell_oa != "opin" && inst_cell_oa != "iopin")) {
                // get view
                oa::oaString inst_view_oa;
                inst_ptr->getViewName(ns_cdba, inst_view_oa);

                // get instance name
                inst_ptr->getName(ns_cdba, inst_name_oa);

                // get transform
                oa::oaTransform xform;
                inst_ptr->getTransform(xform);

                // create schematic instance
                ans.inst_list.emplace_back(formatter.get_name_unit(std::string(inst_name_oa)), std::string(inst_lib_oa),
                                           std::string(inst_cell_oa), std::string(inst_view_oa),
                                           cbag::Transform(xform.xOffset(), xform.yOffset(),
                                                           convert_orient(xform.orient())));
                // get instance pointer
                cbag::CSchInstance *sinst_ptr = &ans.inst_list.back();

                // get parameters
                if (inst_ptr->hasProp()) {
                    oa::oaIter<oa::oaProp> prop_iter(inst_ptr->getProps());
                    oa::oaProp *prop_ptr;
                    while ((prop_ptr = prop_iter.getNext()) != nullptr) {
                        add_param(sinst_ptr->params, prop_ptr);
                    }
                }

                // get instance terminal connections
                uint32_t inst_size = sinst_ptr->size();
                oa::oaIter<oa::oaInstTerm> iterm_iter(inst_ptr->getInstTerms(oacInstTermIterAll));
                oa::oaInstTerm *iterm_ptr;
                while ((iterm_ptr = iterm_iter.getNext()) != nullptr) {
                    oa::oaTerm *term_ptr = iterm_ptr->getTerm();
                    cbag::Name *term_name_ptr;
                    term_ptr->getName(ns_cdba, tmp_str);
                    switch (term_ptr->getTermType()) {
                        case oa::oacInputTermType :
                            sinst_ptr->in_pins.push_back(make_name(std::string(tmp_str)));
                            term_name_ptr = &sinst_ptr->in_pins.back();
                            break;
                        case oa::oacOutputTermType :
                            sinst_ptr->out_pins.push_back(make_name(std::string(tmp_str)));
                            term_name_ptr = &sinst_ptr->out_pins.back();
                            break;
                        case oa::oacInputOutputTermType :
                            sinst_ptr->io_pins.push_back(make_name(std::string(tmp_str)));
                            term_name_ptr = &sinst_ptr->io_pins.back();
                            break;
                        default:
                            errstream << "Instance " << inst_name_oa << " pin " << tmp_str
                                      << " has invalid terminal type: " << term_ptr->getTermType().getName();
                            throw std::invalid_argument(errstream.str());
                    }

                    std::cout << "  Terminal: " << tmp_str << std::endl;
                    std::cout << "    numBits: " << iterm_ptr->getNumBits() << std::endl;
                    iterm_ptr->getNet()->getName(ns_cdba, tmp_str);
                    std::cout << "    Net: " << tmp_str << std::endl;
                }
            }
        }

        // sort pins and instances so we have consistent order
        std::sort(ans.in_pins.begin(), ans.in_pins.end());
        std::sort(ans.out_pins.begin(), ans.out_pins.end());
        std::sort(ans.io_pins.begin(), ans.io_pins.end());
        std::sort(ans.inst_list.begin(), ans.inst_list.end());

        // test range
        cbag::Range test(10, 0, 2);

        for (const auto ele : test) {
            std::cout << "Range index: " << ele << std::endl;
        }

        // close design and return master
        dsn_ptr->close();
        return ans;
    }

    void Library::close() {
        if (is_open) {
            tech_ptr->close();
            lib_ptr->close();

            is_open = false;
        }

    }

    cbag::Name Library::make_name(std::string && std_str) {
        return formatter.get_name(std_str);
    }

    void add_param(cbag::ParamMap &params, oa::oaProp *prop_ptr) {
        // get parameter name
        oa::oaString tmp_str;
        prop_ptr->getName(tmp_str);
        std::string key(tmp_str);

        // get parameter value
        // NOTE: static_cast for down-casting is bad, but openaccess API sucks...
        // use NOLINT to suppress IDE warnings
        oa::oaType ptype = prop_ptr->getType();
        switch (ptype) {
            case oa::oacStringPropType : {
                prop_ptr->getValue(tmp_str);
                std::string vals(tmp_str);
                params.emplace(key, vals);
                break;
            }
            case oa::oacIntPropType : {
                params.emplace(key, static_cast<oa::oaIntProp *>(prop_ptr)->getValue()); // NOLINT
                break;
            }
            case oa::oacDoublePropType : {
                params.emplace(key,
                               static_cast<oa::oaDoubleProp *>(prop_ptr)->getValue()); // NOLINT
                break;
            }
            case oa::oacFloatPropType : {
                double vald = static_cast<oa::oaFloatProp *>(prop_ptr)->getValue(); // NOLINT
                params.emplace(key, vald);
                break;
            }
            default : {
                std::ostringstream stream;
                stream << "Unsupport OA property type: " << ptype.getName() << ".  See developer.";
                throw std::invalid_argument(stream.str());
            }
        }
    }

}