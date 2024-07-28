/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package hepta.rxposed.manager.fragment

import android.annotation.SuppressLint
import android.content.Context
import android.os.Bundle
import android.text.InputType
import android.view.*
import android.widget.*
import androidx.core.view.MenuHost
import androidx.core.view.MenuProvider
import androidx.fragment.app.Fragment
import androidx.lifecycle.Lifecycle
import androidx.navigation.fragment.findNavController
import com.afollestad.materialdialogs.LayoutMode
import com.afollestad.materialdialogs.MaterialDialog
import com.afollestad.materialdialogs.bottomsheets.BottomSheet
import com.afollestad.materialdialogs.input.input
import com.afollestad.materialdialogs.list.listItems
import hepta.rxposed.manager.R
import hepta.rxposed.manager.util.CheckTool11
import hepta.rxposed.manager.util.InjectTool
import hepta.rxposed.manager.widget.DialogUtil


/**
 * Fragment used to show how to navigate to another destination
 */
class HomeFragment : Fragment() {
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {

        return inflater.inflate(R.layout.fragment_home, container, false)
    }

    @SuppressLint("CheckResult")
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        rxposed_activity_ui_init()
        val btnModules = view.findViewById<Button>(R.id.btn_modules)
        btnModules?.setOnClickListener {
            findNavController().navigate(R.id.modules_dest, null)
        }

        val btnSetting = view.findViewById<Button>(R.id.btn_setting)
        btnSetting?.setOnClickListener {

            findNavController().navigate(R.id.settings_dest, null)

        }
        val btnFramework = view.findViewById<Button>(R.id.btn_framework)
        btnFramework?.setOnClickListener {
            findNavController().navigate(R.id.pluginject_dest, null)

        }


        val menuHost: MenuHost = requireActivity()
        menuHost.addMenuProvider(object : MenuProvider {
            override fun onCreateMenu(menu: Menu, menuInflater: MenuInflater) {
                menuInflater.inflate(R.menu.main_menu, menu)

            }

            override fun onMenuItemSelected(menuItem: MenuItem): Boolean {
                when(menuItem.itemId){
                    R.id.id_toolbar_option->{
                        MaterialDialog(requireContext(), BottomSheet(LayoutMode.WRAP_CONTENT)).show {
                            listItems(R.array.rxposetInjectOptions, waitForPositiveButton = false) { _, index, text ->

                                when(index){
                                    0 -> DialogUtil.DidalogSimple(requireContext(),R.string.zygoteMessage, {
                                        InjectTool.zygote_reboot()
                                    })
                                    1 -> DialogUtil.DidalogSimple(requireContext(),R.string.rebootMessage, {
                                        InjectTool.Application_reboot()
                                    })
                                    2 -> DialogUtil.DidalogSimple(requireContext(),R.string.injectTestMessage, {
                                        InjectTool.inject_text()
                                    })
                                    3 ->  {
                                        cancel()
                                        findNavController().navigate(R.id.helplog_dest, null)
                                    }
                                }

                            }
                            positiveButton(android.R.string.cancel)
                        }
                    }
                }
                return false
            }

        },viewLifecycleOwner, Lifecycle.State.RESUMED)
    }


    private fun rxposed_activity_ui_init() {

        val btn_activity = view?.findViewById<Button>(R.id.btn_activity)
        val Image_icon = view?.findViewById<ImageView>(R.id.status_icon)
        val Text_status = view?.findViewById<TextView>(R.id.status_text)

        var activity = CheckTool11.get_rxposed_status();

        if (activity){
            btn_activity?.visibility=View.INVISIBLE
            Text_status?.visibility=View.INVISIBLE
        }else{
            Image_icon?.setImageResource(R.drawable.ic_error)
            btn_activity?.setOnClickListener {

                DialogUtil.DidalogSimple(requireContext(),R.string.activityMessage, {
                    InjectTool.Start()
                    DialogUtil.DidalogSimple(requireContext(),R.string.rxrebootMessage, {
                        InjectTool.Application_reboot()
                    })
                })
            }
        }
    }

}
