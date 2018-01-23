/*
    Aeskulap Configuration - persistent configuration interface library
    Copyright (C) 2005  Alexander Pipelka

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    Alexander Pipelka
*/

#include <giomm.h>
#include <glibmm.h>
#include <cstdio>
#include <iostream>
#include "aconfiguration.h"

namespace Aeskulap {

using Gio::Settings;
using PSettings = Glib::RefPtr<Settings>;

struct ConfigurationImpl {

        ConfigurationImpl();

        PSettings settings_prefs;
        PSettings settings_presets;

        bool has_modality(const Glib::ustring& modality);
};

ConfigurationImpl::ConfigurationImpl():
        settings_prefs(Settings::create("org.gnu.aeskulap")),
        settings_presets(Settings::create("org.gnu.aeskulap.presets"))
{

}

bool ConfigurationImpl::has_modality(const Glib::ustring& modality)
{
        auto children = settings_presets->list_children();
        bool has_modality = false;
        for (auto c = children.begin(); !has_modality && c != children.end(); ++c)
                has_modality = (*c == modality);
        return has_modality;
}

Configuration::Configuration() {
        std::cout << "Gio::Settings init" << std::endl;
        Gio::init();

        impl = new ConfigurationImpl();

        if (!impl->has_modality("CT"))
                add_default_presets_ct();
}

std::string Configuration::get_local_aet() {

        Glib::ustring local_aet = impl->settings_prefs->get_string("local-aet");

        if(local_aet.empty()) {
                local_aet = "AESKULAP";
                set_local_aet(local_aet);
        }

        return std::string(local_aet.c_str());
}

void Configuration::set_local_aet(const std::string& aet) {
        impl->settings_prefs->set_string("local-aet", aet);
}

unsigned int Configuration::get_local_port() {

        gint local_port = impl->settings_prefs->get_int("local-port");

        if(local_port <= 0) {
                local_port = 6000;
                set_local_port(local_port);
        }

        return (unsigned int)local_port;
}

void Configuration::set_local_port(unsigned int port) {
        if(port <= 0) {
                port = 6000;
        }
        impl->settings_prefs->set_int("local-port", (gint)port);
}

std::string Configuration::get_encoding() {
        Glib::ustring charset = impl->settings_prefs->get_string("characterset");

        if(charset.empty()) {
                charset = "ISO_IR 100";
                set_encoding(charset);
        }

        return charset.c_str();
}

void Configuration::set_encoding(const std::string& encoding) {
        impl->settings_prefs->set_string("characterset", encoding);
}

std::vector<int> convert_to_int_array(const std::vector<Glib::ustring>& in) {
        std::vector<int> result(in.size());

        transform(in.begin(), in.end(), result.begin(), [](const Glib::ustring& x) {
                std::istringstream s(x.c_str());
                int o;
                s >> o;
                return o;
        });
        return result;
}

std::vector<bool> convert_to_bool_array(const std::vector<Glib::ustring>& in) {
        std::vector<bool> result(in.size());

        transform(in.begin(), in.end(), result.begin(), [](const Glib::ustring& x) {
                return x == "true";
        });
        return result;
}


Configuration::ServerList* Configuration::get_serverlist() {
        Configuration::ServerList* list = new Configuration::ServerList;

        std::vector<Glib::ustring> aet_list         = impl->settings_prefs->get_string_array("server-aet");
        std::vector<int> port_list                  = convert_to_int_array(impl->settings_prefs->get_string_array("server-port"));
        std::vector<Glib::ustring> hostname_list    = impl->settings_prefs->get_string_array("server-hostname");
        std::vector<Glib::ustring> description_list = impl->settings_prefs->get_string_array("server-description");
        std::vector<Glib::ustring> group_list       = impl->settings_prefs->get_string_array("server-group");
        std::vector<bool> lossy_list                = convert_to_bool_array(impl->settings_prefs->get_string_array("server-lossy"));
        std::vector<bool> relational_list           = convert_to_bool_array(impl->settings_prefs->get_string_array("server-relational"));

        auto a = aet_list.begin();
        auto p = port_list.begin();
        auto h = hostname_list.begin();
        auto d = description_list.begin();
        auto g = group_list.begin();
        auto l = lossy_list.begin();
        auto r = relational_list.begin();

        for(; h != hostname_list.end() && a != aet_list.end()&& p != port_list.end(); a++, p++, h++) {

                std::string servername;
                if(d != description_list.end()) {
                        servername = *d;
                        d++;
                }
                else {
                        char buffer[50];
                        snprintf(buffer, sizeof(buffer), "Server%li", list->size()+1);
                        servername = buffer;
                }

                ServerData& s = (*list)[servername];
                s.m_aet = *a;
                s.m_port = *p;
                s.m_hostname = *h;
                s.m_name = servername;
                s.m_lossy = false;
                s.m_relational = false;

                if(g != group_list.end()) {
                        s.m_group = *g;
                        g++;
                }

                if ( l != lossy_list.end()) {
                        s.m_lossy = *l;
                        ++l;
                }

                if(r != relational_list.end()) {
                        s.m_relational = *r;
                        r++;
                }

        }

        return list;
}

void Configuration::set_serverlist(std::vector<ServerData>& list) {

        std::vector<Glib::ustring> aet_list;
        std::vector<Glib::ustring> hostname_list;
        std::vector<Glib::ustring> port_list;
        std::vector<Glib::ustring> description_list;
        std::vector<Glib::ustring> group_list;
        std::vector<Glib::ustring> lossy_list;
        std::vector<Glib::ustring> relational_list;

        std::vector<ServerData>::iterator i;
        for(i = list.begin(); i != list.end(); i++) {
                aet_list. push_back(i->m_aet);
                hostname_list.push_back(i->m_hostname);
                port_list.push_back( Glib::ustring::compose("%1", i->m_port));
                description_list.push_back(i->m_name);
                group_list.push_back(i->m_group);
                lossy_list.push_back(i->m_lossy ? "true": "false");
                relational_list.push_back(i->m_relational ? "true": "false");
        }

        impl->settings_prefs->set_string_array("server-aet", aet_list);
        impl->settings_prefs->set_string_array("server-hostname", hostname_list);
        impl->settings_prefs->set_string_array("server-port", port_list);
        impl->settings_prefs->set_string_array("server-description", description_list);
        impl->settings_prefs->set_string_array("server-group", group_list);
        impl->settings_prefs->set_string_array("server-lossy", lossy_list);
        impl->settings_prefs->set_string_array("server-relational", relational_list);
}

bool Configuration::get_windowlevel(const Glib::ustring&  modality,
                                    const Glib::ustring& desc, WindowLevel& w) {

        auto modality_settings = impl->settings_presets->get_child(modality);
        if (!modality_settings) {
                g_warning("Modality %s not found", modality.c_str());
                return false;
        }

        auto tissue_settings = modality_settings->get_child(desc);
        if (!tissue_settings) {
                g_warning("tissue setting for %s not found in %s", desc.c_str(),
                          modality.c_str());
                return false;
        }

        w.modality = modality;
        w.description = desc;
        w.center = tissue_settings->get_int("center");
        w.width = tissue_settings->get_int("width");

        return true;
}

static PSettings get_child_tree(PSettings settings, const Glib::ustring& key,
                                const Glib::ustring& scheme)
{
        std::string pp = settings->property_path();
        pp.append(key).append("/");
        g_message("Get child tree at %s", pp.c_str());
        return Settings::create(scheme, pp);
}


bool Configuration::get_windowlevel_list(const Glib::ustring& modality,
                WindowLevelList& list) {

        if(modality.empty()) {
                g_warning("No modality given");
                return false;
        }

        std::vector<Glib::ustring> supported_modalities =
                impl->settings_presets->get_string_array("modalities");
        auto m = find(supported_modalities.begin(), supported_modalities.end(),
                      modality);
        if (m == supported_modalities.end()) {
                g_warning("Modality %s not found in presets", modality.c_str());
                return false;
        }

        auto modality_settings = get_child_tree(impl->settings_presets, modality,
                                                "org.gnu.aeskulap.presets.modality");

        std::vector<Glib::ustring> supported_tissues =
                modality_settings->get_string_array("tissue-types");

        for(const auto& tissue:  supported_tissues) {
                WindowLevel w;
                w.modality = modality;
                w.description = tissue;

                auto tissue_settings = get_child_tree(modality_settings, tissue,
                                                      "org.gnu.aeskulap.presets.modality.tissue");

                w.center = tissue_settings->get_int("center");
                w.width = tissue_settings->get_int("width");
                list[tissue] = w;
        }
        return true;
}

bool Configuration::set_windowlevel(const WindowLevel& w) {

        std::vector<Glib::ustring> supported_modalities =
                impl->settings_presets->get_string_array("modalities");
        if (find(supported_modalities.begin(), supported_modalities.end(),
                        w.modality) == supported_modalities.end()) {
                supported_modalities.push_back(w.modality);
                impl->settings_presets->set_string_array("modalities", supported_modalities);
        }

        auto modality_settings = get_child_tree(impl->settings_presets, w.modality,
                                                "org.gnu.aeskulap.presets.modality");
        std::vector<Glib::ustring> tissues =
                modality_settings->get_string_array("tissue-types");

        if (find(tissues.begin(), tissues.end(), w.description) == tissues.end()) {
                tissues.push_back(w.description);
                modality_settings->set_string_array("tissue-types", tissues);
        }

        auto tissue_settings = get_child_tree(modality_settings, w.description,
                                              "org.gnu.aeskulap.presets.modality.tissue");

        tissue_settings->set_int("center", w.center);
        tissue_settings->set_int("width", w.width);

        return true;
}



bool Configuration::set_windowlevel_list(const Glib::ustring& modality,
                WindowLevelList& list) {

        std::vector<Glib::ustring> supported_modalities =
                impl->settings_presets->get_string_array("modalities");
        if (find(supported_modalities.begin(), supported_modalities.end(),
                        modality) == supported_modalities.end()) {
                supported_modalities.push_back(modality);
                impl->settings_presets->set_string_array("modalities", supported_modalities);
        }


        auto modality_settings = get_child_tree(impl->settings_presets, modality,
                                                "org.gnu.aeskulap.presets.modality");
        std::vector<Glib::ustring> tissues =
                modality_settings->get_string_array("tissue-types");


        for(auto i = list.begin(); i != list.end(); i++) {
                auto& t = i->second.description;
                if (find(tissues.begin(), tissues.end(), t) == tissues.end())
                        tissues.push_back(t);

                i->second.modality = modality;

                auto tissue_settings = get_child_tree(modality_settings, t,
                                                      "org.gnu.aeskulap.presets.modality.tissue");

                tissue_settings->set_int("center", i->second.center);
                tissue_settings->set_int("width", i->second.width);
        }
        modality_settings->set_string_array("tissue-types", tissues);

        return true;
}

bool Configuration::unset_windowlevels(const Glib::ustring& modality) {

        std::vector<Glib::ustring> supported_modalities =
                impl->settings_presets->get_string_array("modalities");
        auto m = find(supported_modalities.begin(), supported_modalities.end(),
                      modality);
        if (m == supported_modalities.end())
                return true;

        auto modality_settings = get_child_tree(impl->settings_presets, modality,
                                                "org.gnu.aeskulap.presets.modality");
        std::vector<Glib::ustring> tissues =
                modality_settings->get_string_array("tissue-types");

        for (auto& t: tissues) {
                auto tissue_settings = get_child_tree(modality_settings, t,
                                                      "org.gnu.aeskulap.presets.modality.tissue");
                tissue_settings->reset("center");
                tissue_settings->reset("width");
        }

        return true;
}

} // namespace Aeskulap
